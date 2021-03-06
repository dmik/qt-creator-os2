/**************************************************************************
**
** This file is part of Qt Creator
**
** Copyright (c) 2011 Nokia Corporation and/or its subsidiary(-ies).
**
** Contact: Nokia Corporation (info@qt.nokia.com)
**
**
** GNU Lesser General Public License Usage
**
** This file may be used under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation and
** appearing in the file LICENSE.LGPL included in the packaging of this file.
** Please review the following information to ensure the GNU Lesser General
** Public License version 2.1 requirements will be met:
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights. These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** Other Usage
**
** Alternatively, this file may be used in accordance with the terms and
** conditions contained in a signed written agreement between you and Nokia.
**
** If you have questions regarding the use of this file, please contact
** Nokia at info@qt.nokia.com.
**
**************************************************************************/

import Qt 4.7
import Bauhaus 1.0

QWidget {
    layout: QVBoxLayout {
        topMargin: 0
        bottomMargin: 0
        leftMargin: 0
        rightMargin: 0
        spacing: 0
        FlickableGroupBox {
        }
        GroupBox {
            finished: finishedNotify;
            caption: qsTr("Grid View")
            layout: VerticalLayout {
                IntEditor {
                    backendValue: backendValues.cacheBuffer
                    caption: qsTr("Cache")
                    toolTip: qsTr("Cache buffer")
                    baseStateFlag: isBaseState;
                    step: 1;
                    minimumValue: 0;
                    maximumValue: 1000;
                }
                IntEditor {
                    backendValue: backendValues.cellHeight
                    caption: qsTr("Cell height")
                    baseStateFlag: isBaseState;
                    step: 1;
                    minimumValue: 0;
                    maximumValue: 200;
                }
                IntEditor {
                    backendValue: backendValues.cellWidth
                    caption: qsTr("Cell width")
                    baseStateFlag: isBaseState;
                    step: 1;
                    minimumValue: 0;
                    maximumValue: 1000;
                }
                QWidget {
                    layout: HorizontalLayout {
                        Label {
                            text: qsTr("Flow")
                        }

                        ComboBox {
                            baseStateFlag: isBaseState
                            items : { ["LeftToRight", "TopToBottom"] }
                            currentText: backendValues.flow.value;
                            onItemsChanged: {
                                currentText =  backendValues.flow.value;
                            }
                            backendValue: backendValues.flow
                        }
                    }
                } //QWidget

                IntEditor {
                    backendValue: backendValues.keyNavigationWraps
                    caption: qsTr("Resize wraps")
                    toolTip: qsTr("Determines whether the grid wraps key navigation.")
                    baseStateFlag: isBaseState;
                    step: 1;
                    minimumValue: 0;
                    maximumValue: 1000;
                }
                //                Qt namespace enums not supported by the rewriter
                //                QWidget {
                //                    layout: HorizontalLayout {
                //                        Label {
                //                            text: qsTr("Layout direction")
                //                        }

                //                        ComboBox {
                //                            baseStateFlag: isBaseState
                //                            items : { ["LeftToRight", "RightToLeft"] }
                //                            currentText: backendValues.layoutDirection.value;
                //                            onItemsChanged: {
                //                                currentText =  backendValues.layoutDirection.value;
                //                            }
                //                            backendValue: backendValues.layoutDirection
                //                        }
                //                    }
                //                } //QWidget
                QWidget {
                    layout: HorizontalLayout {
                        Label {
                            text: qsTr("Snap mode")
                            toolTip: qsTr("Determines how the view scrolling will settle following a drag or flick.")
                        }

                        ComboBox {
                            baseStateFlag: isBaseState
                            items : { ["NoSnap", "SnapToRow", "SnapOneRow"] }
                            currentText: backendValues.snapMode.value;
                            onItemsChanged: {
                                currentText =  backendValues.snapMode.value;
                            }
                            backendValue: backendValues.snapMode
                        }
                    }
                } //QWidget
            }
        }
        GroupBox {
            finished: finishedNotify;
            caption: qsTr("Grid View Highlight")
            layout: VerticalLayout {
                QWidget {
                    layout: HorizontalLayout {
                        Label {
                            text: qsTr("Range")
                            toolTip: qsTr("Highlight range")
                        }

                        ComboBox {
                            baseStateFlag: isBaseState
                            items : { ["NoHighlightRange", "ApplyRange", "StrictlyEnforceRange"] }
                            currentText: backendValues.highlightRangeMode.value;
                            onItemsChanged: {
                                currentText =  backendValues.highlightRangeMode.value;
                            }
                            backendValue: backendValues.highlightRangeMode
                        }
                    }
                } //QWidget
                IntEditor {
                    backendValue: backendValues.highlightMoveDuration
                    caption: qsTr("Move duration")
                    toolTip: qsTr("Move animation duration of the highlight delegate.")
                    baseStateFlag: isBaseState;
                    step: 1;
                    minimumValue: 0;
                    maximumValue: 1000;
                }
                IntEditor {
                    backendValue: backendValues.highlightMoveSpeed
                    caption: qsTr("Move speed")
                    toolTip: qsTr("Move animation speed of the highlight delegate.")
                    baseStateFlag: isBaseState;
                    step: 1;
                    minimumValue: 0;
                    maximumValue: 1000;
                }
                IntEditor {
                    backendValue: backendValues.preferredHighlightBegin
                    caption: qsTr("Preferred begin")
                    toolTip: qsTr("Preferred highlight begin - must be smaller than Preferred end.")
                    baseStateFlag: isBaseState;
                    step: 1;
                    minimumValue: 0;
                    maximumValue: 1000;
                }
                IntEditor {
                    backendValue: backendValues.preferredHighlightEnd
                    caption: qsTr("Preferred end")
                    toolTip: qsTr("Preferred highlight end - must be larger than Preferred begin.")
                    baseStateFlag: isBaseState;
                    step: 1;
                    minimumValue: 0;
                    maximumValue: 1000;
                }
                QWidget {  // 1
                    layout: HorizontalLayout {

                        Label {
                            text: qsTr("Follows current")
                        }
                        CheckBox {
                            backendValue: backendValues.highlightFollowsCurrentItem
                            toolTip: qsTr("Determines whether the highlight is managed by the view.")
                            baseStateFlag: isBaseState;
                            checkable: True
                        }
                    }
                }
            }
        }
        QScrollArea {
        }
    }
}
