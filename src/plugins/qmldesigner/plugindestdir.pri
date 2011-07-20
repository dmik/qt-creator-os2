macx {
  DESTDIR = $$IDE_LIBRARY_PATH/QmlDesigner
} else {
  DESTDIR = $$IDE_BUILD_TREE/$$IDE_LIBRARY_BASENAME/qmldesigner
}

!macx {
    target.path = /$$IDE_LIBRARY_BASENAME/qmldesigner
    INSTALLS += target
}
