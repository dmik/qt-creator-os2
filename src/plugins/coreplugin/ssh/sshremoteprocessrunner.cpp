#include "sshremoteprocessrunner.h"

#define ASSERT_STATE(states) assertState(states, Q_FUNC_INFO)

namespace Core {

class SshRemoteProcessRunnerPrivate : public QObject
{
    Q_OBJECT
public:
    SshRemoteProcessRunnerPrivate(const SshConnectionParameters &params,
        QObject *parent);
    SshRemoteProcessRunnerPrivate(const SshConnection::Ptr &connection,
        QObject *parent);
    void run(const QByteArray &command);
    QByteArray command() const { return m_command; }

    const SshConnection::Ptr m_connection;
    SshRemoteProcess::Ptr m_process;

signals:
    void connectionError(Core::SshError);
    void processStarted();
    void processOutputAvailable(const QByteArray &output);
    void processErrorOutputAvailable(const QByteArray &output);
    void processClosed(int exitStatus);

private slots:
    void handleConnected();
    void handleConnectionError(Core::SshError error);
    void handleDisconnected();
    void handleProcessStarted();
    void handleProcessFinished(int exitStatus);

private:
    enum State { Inactive, Connecting, Connected, ProcessRunning };

    void setState(State state);
    void assertState(const QList<State> &allowedStates, const char *func);
    void assertState(State allowedState, const char *func);

    State m_state;
    QByteArray m_command;
    const SshConnectionParameters m_params;
};


SshRemoteProcessRunnerPrivate::SshRemoteProcessRunnerPrivate(const SshConnectionParameters &params,
    QObject *parent)
    : QObject(parent),
      m_connection(SshConnection::create()),
      m_state(Inactive),
      m_params(params)
{
}

SshRemoteProcessRunnerPrivate::SshRemoteProcessRunnerPrivate(const SshConnection::Ptr &connection,
    QObject *parent)
    : QObject(parent),
      m_connection(connection),
      m_state(Inactive),
      m_params(connection->connectionParameters())
{
}

void SshRemoteProcessRunnerPrivate::run(const QByteArray &command)
{
    ASSERT_STATE(Inactive);
    setState(Connecting);

    m_command = command;
    connect(m_connection.data(), SIGNAL(error(Core::SshError)),
        SLOT(handleConnectionError(Core::SshError)));
    connect(m_connection.data(), SIGNAL(disconnected()),
        SLOT(handleDisconnected()));
    if (m_connection->state() == SshConnection::Connected) {
        handleConnected();
    } else {
        connect(m_connection.data(), SIGNAL(connected()),
            SLOT(handleConnected()));
        m_connection->connectToHost(m_params);
    }
}

void SshRemoteProcessRunnerPrivate::handleConnected()
{
    ASSERT_STATE(Connecting);
    setState(Connected);

    m_process = m_connection->createRemoteProcess(m_command);
    connect(m_process.data(), SIGNAL(started()), SLOT(handleProcessStarted()));
    connect(m_process.data(), SIGNAL(closed(int)),
        SLOT(handleProcessFinished(int)));
    connect(m_process.data(), SIGNAL(outputAvailable(QByteArray)),
        SIGNAL(processOutputAvailable(QByteArray)));
    connect(m_process.data(), SIGNAL(errorOutputAvailable(QByteArray)),
        SIGNAL(processErrorOutputAvailable(QByteArray)));
    m_process->start();
}

void SshRemoteProcessRunnerPrivate::handleConnectionError(Core::SshError error)
{
    handleDisconnected();
    emit connectionError(error);
}

void SshRemoteProcessRunnerPrivate::handleDisconnected()
{
    ASSERT_STATE(QList<State>() << Connecting << Connected << ProcessRunning);
    setState(Inactive);
}

void SshRemoteProcessRunnerPrivate::handleProcessStarted()
{
    ASSERT_STATE(Connected);
    setState(ProcessRunning);

    emit processStarted();
}

void SshRemoteProcessRunnerPrivate::handleProcessFinished(int exitStatus)
{
    switch (exitStatus) {
    case SshRemoteProcess::FailedToStart:
        ASSERT_STATE(Connected);
        break;
    case SshRemoteProcess::KilledBySignal:
    case SshRemoteProcess::ExitedNormally:
        ASSERT_STATE(ProcessRunning);
        break;
    default:
        Q_ASSERT_X(false, Q_FUNC_INFO, "Impossible exit status.");
    }
    setState(Inactive);
    emit processClosed(exitStatus);
}

void SshRemoteProcessRunnerPrivate::setState(State state)
{
    if (m_state != state) {
        m_state = state;
        if (m_state == Inactive) {
            if (m_process)
                disconnect(m_process.data(), 0, this, 0);
            disconnect(m_connection.data(), 0, this, 0);
        }
    }
}

void SshRemoteProcessRunnerPrivate::assertState(const QList<State> &allowedStates,
    const char *func)
{
    if (!allowedStates.contains(m_state))
        qWarning("Unexpected state %d in function %s", m_state, func);
}

void SshRemoteProcessRunnerPrivate::assertState(State allowedState,
    const char *func)
{
    assertState(QList<State>() << allowedState, func);
}


SshRemoteProcessRunner::Ptr SshRemoteProcessRunner::create(const SshConnectionParameters &params)
{
    return SshRemoteProcessRunner::Ptr(new SshRemoteProcessRunner(params));
}

SshRemoteProcessRunner::Ptr SshRemoteProcessRunner::create(const SshConnection::Ptr &connection)
{
    return SshRemoteProcessRunner::Ptr(new SshRemoteProcessRunner(connection));
}

SshRemoteProcessRunner::SshRemoteProcessRunner(const SshConnectionParameters &params)
    : d(new SshRemoteProcessRunnerPrivate(params, this))
{
    init();
}

SshRemoteProcessRunner::SshRemoteProcessRunner(const SshConnection::Ptr &connection)
    : d(new SshRemoteProcessRunnerPrivate(connection, this))
{
    init();
}

void SshRemoteProcessRunner::init()
{
    connect(d, SIGNAL(connectionError(Core::SshError)),
        SIGNAL(connectionError(Core::SshError)));
    connect(d, SIGNAL(processStarted()), SIGNAL(processStarted()));
    connect(d, SIGNAL(processClosed(int)), SIGNAL(processClosed(int)));
    connect(d, SIGNAL(processOutputAvailable(QByteArray)),
        SIGNAL(processOutputAvailable(QByteArray)));
    connect(d, SIGNAL(processErrorOutputAvailable(QByteArray)),
        SIGNAL(processErrorOutputAvailable(QByteArray)));
}

void SshRemoteProcessRunner::run(const QByteArray &command)
{
    d->run(command);
}

QByteArray SshRemoteProcessRunner::command() const { return d->command(); }
SshConnection::Ptr SshRemoteProcessRunner::connection() const { return d->m_connection; }
SshRemoteProcess::Ptr SshRemoteProcessRunner::process() const { return d->m_process; }

} // namespace Core


#include "sshremoteprocessrunner.moc"