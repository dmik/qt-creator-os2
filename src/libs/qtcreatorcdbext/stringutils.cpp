/**************************************************************************
**
** This file is part of Qt Creator
**
** Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
**
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** Commercial Usage
**
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Commercial License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Nokia.
**
** GNU Lesser General Public License Usage
**
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** If you are unsure which license is appropriate for your use, please
** contact the sales department at http://qt.nokia.com/contact.
**
**************************************************************************/

#include "stringutils.h"

#include <cctype>
#include <iostream>
#include <sstream>

static const char whiteSpace[] = " \t\r\n";

void trimFront(std::string &s)
{
    if (s.empty())
        return;
    std::string::size_type pos = s.find_first_not_of(whiteSpace);
    if (pos == 0)
        return;
    if (pos == std::string::npos) { // All blanks?!
        s.clear();
    } else {
        s.erase(0, pos);
    }
}

void trimBack(std::string &s)
{
    if (s.empty())
        return;
    std::string::size_type pos = s.find_last_not_of(whiteSpace);
    if (pos == std::string::npos) { // All blanks?!
        s.clear();
    } else {
        if (++pos != s.size())
            s.erase(pos, s.size() - pos);
    }
}

void simplify(std::string &s)
{
    trimFront(s);
    trimBack(s);
    if (s.empty())
        return;

    // 1) All blanks
    const std::string::size_type size1 = s.size();
    std::string::size_type pos = 0;
    for ( ; pos < size1; pos++)
        if (std::isspace(s.at(pos)))
                s[pos] = ' ';
    // 2) Simplify
    for (pos = 0; pos < s.size(); ) {
        std::string::size_type blankpos = s.find(' ', pos);
        if (blankpos == std::string::npos)
            break;
        std::string::size_type tokenpos = blankpos + 1;
        while (tokenpos < s.size() && s.at(tokenpos) == ' ')
            tokenpos++;
        if (tokenpos - blankpos > 1)
            s.erase(blankpos, tokenpos - blankpos - 1);
        pos = blankpos + 1;
    }
}

void replace(std::wstring &s, wchar_t before, wchar_t after)
{
    const std::wstring::size_type size = s.size();
    for (std::wstring::size_type i = 0; i < size; i++)
        if (s.at(i) == before)
            s[i] = after;
}

bool endsWith(const std::string &haystack, const char *needle)
{
    const size_t needleLen = strlen(needle);
    const size_t haystackLen = haystack.size();
    if (needleLen > haystackLen)
        return false;
    return haystack.compare(haystackLen - needleLen, needleLen, needle) == 0;
}

static inline void formatGdbmiChar(std::ostream &str, wchar_t c)
{
    switch (c) {
    case L'\n':
        str << "\\n";
        break;
    case L'\t':
        str << "\\t";
        break;
    case L'\r':
        str << "\\r";
        break;
    case L'\\':
    case L'"':
        str << '\\' << char(c);
        break;
    default:
        if (c < 128) {
            str << char(c);
        } else {
            // Always pad up to 3 digits in case a digit follows
            const char oldFill = str.fill('0');
            str << '\\' << std::oct;
            str.width(3);
            str << unsigned(c) << std::dec;
            str.fill(oldFill);
        }
        break;
    }
}

// Stream  a wstring onto a char stream doing octal escaping
// suitable for GDBMI.

void gdbmiStringFormat::format(std::ostream &str) const
{
    const std::string::size_type size = m_s.size();
    for (std::string::size_type i = 0; i < size; i++)
        formatGdbmiChar(str, wchar_t(m_s.at(i)));
}

void gdbmiWStringFormat::format(std::ostream &str) const
{
    const std::wstring::size_type size = m_w.size();
    for (std::wstring::size_type i = 0; i < size; i++)
        formatGdbmiChar(str, m_w.at(i));
}

std::string wStringToGdbmiString(const std::wstring &w)
{
    std::ostringstream str;
    str << gdbmiWStringFormat(w);
    return str.str();
}

std::string wStringToString(const std::wstring &w)
{
    if (w.empty())
        return std::string();
    const std::string::size_type size = w.size();
    std::string rc;
    rc.reserve(size);
    for (std::string::size_type i = 0; i < size; i++)
        rc.push_back(char(w.at(i)));
    return rc;
}

std::wstring stringToWString(const std::string &w)
{
    if (w.empty())
        return std::wstring();
    const std::wstring::size_type size = w.size();
    std::wstring rc;
    rc.reserve(size);
    for (std::wstring::size_type i = 0; i < size; i++)
        rc.push_back(w.at(i));
    return rc;
}

// Format a map as a GDBMI hash {key="value",..}
void formatGdbmiHash(std::ostream &os, const std::map<std::string, std::string> &m)
{
    typedef std::map<std::string, std::string>::const_iterator It;
    const It begin = m.begin();
    const It cend = m.end();
    os << '{';
    for (It it = begin; it != cend; ++it) {
        if (it != begin)
            os << ',';
        os << it->first << "=\"" << gdbmiStringFormat(it->second) << '"';
    }
    os << '}';
}