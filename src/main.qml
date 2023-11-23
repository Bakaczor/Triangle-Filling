import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Basic

ApplicationWindow {
    id: main_window
    visible: true
    minimumWidth: 800
    maximumWidth: 800
    minimumHeight: 600
    maximumHeight: 600
    title: "Triangle Filling"


    Connections {
        target: SceneManager
        function onImageChanged() {
            image.reload();
        }
        function onLightColorChanged() {
            lightColorRect.color = SceneManager.lightColor
        }
        function onBackColorChanged() {
            backColorRect.color = SceneManager.backColor
        }
        function onUpdateCurrentZ() {
            cpSlider.value = SceneManager.getCurrentZ()
        }
    }

    Rectangle {
        anchors.fill: parent
        color: "lightgrey"

        Row {
            anchors.fill: parent
            anchors.margins: 5
            spacing: 5


            Image {
                id: image
                width: parent.width - 200
                height: parent.height
                source: "image://ImageProvider/image"
                cache: false
                focus: true

                function reload() {
                    var oldSource = source;
                    source = "";
                    source = oldSource;
                }

                MouseArea {
                   id: mouseArea
                   anchors.fill: parent
                   hoverEnabled: true

                   onClicked: {
                        if (SceneManager.isInCPView) {
                            SceneManager.selectPoint(mouseArea.mouseX, mouseArea.mouseY)
                        }
                   }
                }
            }


            Rectangle {
                width: 195
                height: parent.height
                color: "deepskyblue"

                Column {
                    padding: 7
                    spacing: 15
                    Column {
                        focus: false
                        Label {
                            text: "Control points management:"
                        }
                        Row {
                            spacing: 5
                            CheckBox {
                                id: cpCheckBox
                                text: "show"
                                onClicked: {
                                    SceneManager.setIsInCPView (cpCheckBox.checked)
                                }
                            }
                            Slider {
                                id: cpSlider
                                implicitWidth: 100
                                from: 0
                                value: 0
                                to: 1
                                stepSize: 0.01
                                snapMode: Slider.SnapAlways
                                focusPolicy: Qt.NoFocus
                                onPressedChanged: {
                                    if (!cpSlider.pressed) {
                                        SceneManager.modifyPoint(cpSlider.value)
                                    }
                                }
                            }
                        }
                    }
                    Column {
                        focus: false
                        Label {
                            text: "Triangulation:"
                        }
                        Slider {
                            id: triNumSlider
                            implicitWidth: 150
                            from: 2
                            value: 7
                            to: 15
                            stepSize: 1
                            snapMode: Slider.SnapAlways
                            focusPolicy: Qt.NoFocus
                            onMoved: {
                                SceneManager.setTriNum(triNumSlider.value)
                            }
                        }
                        Row {
                            CheckBox {
                                id: triNumCheckBox
                                text: "show grid"
                                height: 10
                                onClicked: {
                                    SceneManager.setShowGrid(triNumCheckBox.checked)
                                }
                            }
                            CheckBox {
                                id: fillCheckBox
                                text: "fill grid"
                                checked: true
                                height: 10
                                onClicked: {
                                    SceneManager.setFillGrid(fillCheckBox.checked)
                                }
                            }
                        }
                    }
                    Row {
                        focus: false
                        spacing: 5
                        Label {
                            text: "Light color:"
                        }
                        Button {
                            id: lightColorButton
                            height: 25
                            width: 25
                            background: Rectangle {
                                id: lightColorRect
                                implicitHeight: parent.height
                                implicitWidth: parent.width
                                opacity: 1
                                border.color: "white"
                                border.width: 2
                                color: "white"
                            }
                            onClicked: {
                                SceneManager.pickLightColor()
                            }
                        }
                    }
                    Row {
                        focus: false
                        spacing: 5
                        Label {
                            text: "Background:"
                        }
                        Button {
                            id: backColorButton
                            height: 25
                            width: 25
                            background: Rectangle {
                                id: backColorRect
                                implicitHeight: parent.height
                                implicitWidth: parent.width
                                opacity: 1
                                border.color: "white"
                                border.width: 2
                                color: "white"
                            }
                            onClicked: {
                                SceneManager.pickBackColor()
                            }
                        }
                        Label {
                            text: "or"
                        }
                        Button {
                            text: "image"
                            height: 25
                            width: 60
                            onClicked: {
                                SceneManager.pickImage()
                            }
                        }
                    }
                    Row {
                        focus: false
                        spacing: 5
                        Label {
                            text: "Texture:"
                        }
                        Button {
                            text: "map"
                            height: 25
                            width: 60
                            onClicked: {
                                SceneManager.pickTexture()
                            }
                        }
                    }
                    Row {
                        focus: false
                        spacing: 5
                        RadioButton {
                            text: "Bezier"
                            checked: true
                            height: 10
                            onClicked: {
                                SceneManager.changeGridType()
                            }
                        }
                        RadioButton {
                            text: "Functional"
                            height: 10
                            onClicked: {
                                SceneManager.changeGridType()
                            }
                        }
                    }
                    Column {
                        focus: false
                        Label {
                            text: "Parameters:"
                        }
                        Grid {
                            columns: 2
                            rows: 5
                            Label {
                                text: "kd:"
                                y: parent.y - 10
                            }
                            Slider {
                                id: kdSlider
                                implicitWidth: 150
                                from: 0
                                value: 0.5
                                to: 1
                                stepSize: 0.01
                                snapMode: Slider.SnapAlways
                                focusPolicy: Qt.NoFocus
                                onMoved: {
                                    SceneManager.setKd(kdSlider.value)
                                }
                            }
                            Label {
                                text: "ks:"
                                y: parent.y + 35
                            }
                            Slider {
                                id: ksSlider
                                implicitWidth: 150
                                from: 0
                                value: 0.5
                                to: 1
                                stepSize: 0.01
                                snapMode: Slider.SnapAlways
                                focusPolicy: Qt.NoFocus
                                onMoved: {
                                    SceneManager.setKs(ksSlider.value)
                                }
                            }
                            Label {
                                text: "m:"
                                y: parent.y + 75
                            }
                            Slider {
                                id: mSlider
                                implicitWidth: 150
                                from: 1
                                value: 50
                                to: 100
                                stepSize: 1
                                snapMode: Slider.SnapAlways
                                focusPolicy: Qt.NoFocus
                                onMoved: {
                                    SceneManager.setM(mSlider.value)
                                }
                            }
                            Label {
                                text: "alfa:"
                                y: parent.y + 115
                            }
                            Slider {
                                id: alfaSlider
                                implicitWidth: 150
                                from: 0
                                value: 0
                                to: 1
                                stepSize: 0.01
                                snapMode: Slider.SnapAlways
                                focusPolicy: Qt.NoFocus
                                onMoved: {
                                    SceneManager.setAlfa(alfaSlider.value)
                                }
                            }
                            Label {
                                text: "beta:"
                                y: parent.y + 155
                            }
                            Slider {
                                id: betaSlider
                                implicitWidth: 150
                                from: 0
                                value: 0
                                to: 1
                                stepSize: 0.01
                                snapMode: Slider.SnapAlways
                                focusPolicy: Qt.NoFocus
                                onMoved: {
                                    SceneManager.setBeta(betaSlider.value)
                                }
                            }
                        }
                    }
                    Column {
                        focus: false
                        spacing: 5
                        Label {
                            text: "Animation:"
                        }
                        Row {
                            spacing: 5
                            Button {
                                text: "play"
                                height: 25
                                width: 60
                                onClicked: {
                                    SceneManager.setIsPlaying(true)
                                }
                            }
                            Button {
                                text: "pause"
                                height: 25
                                width: 60
                                onClicked: {
                                    SceneManager.setIsPlaying(false)
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
