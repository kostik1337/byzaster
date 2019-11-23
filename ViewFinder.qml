import QtQuick 2.12
import QtQuick.Window 2.12

Window {
    id: root
    flags: Qt.Tool | Qt.FramelessWindowHint | Qt.WA_TranslucentBackground | 
           Qt.WindowStaysOnTopHint
    modality: "ApplicationModal"
    title: qsTr("View Finder")
    color: "#00000000"
    
    property rect region: Qt.rect(0, 0, Screen.width, Screen.height)
    property size minViewSize: Qt.size(32, 32);
    property color tint: "#90000000"
    property color borderColor: "#90ffffff"
    property variant aspectRatios: [1/2, 16/9, 4/3, 1/1]
    
    signal selected(rect frame)
    
    onActiveChanged: {
        if (active) 
            region = Qt.rect(0, 0, Screen.width, Screen.height)
    }
    
    MouseArea {
        anchors.fill: parent
        cursorShape: "CrossCursor"
        
        onPressed: {
            region.x = mouseX
            region.y = mouseY
            region.width = 0
            region.heght = 0
        }
        
        onPositionChanged: {            
            region.height = mouseY - region.y
            region.width = mouseX - region.x
            
            if (mouse.modifiers & Qt.ShiftModifier) {
                var max = Math.max(region.width, region.height),
                    ratio = Math.abs(region.width / region.height);
                
                if (ratio > 1) {
                    ratio = aspectRatios.reduce((acc, r) => 
                            Math.abs(ratio - r) < Math.abs(ratio - acc) ? r : acc);
                    
                    region.width = Math.sign(region.width) * max;
                    region.height = Math.sign(region.height) * Math.floor(max / ratio);
                } else {
                    ratio = 1 / ratio;
                    ratio = aspectRatios.reduce((acc, r) => 
                            Math.abs(ratio - r) < Math.abs(ratio - acc) ? r : acc);
                    
                    region.width = Math.sign(region.width) * Math.floor(max / ratio);
                    region.height = Math.sign(region.height) * max;
                }
            }
        }
        
        onReleased: {
            if (Math.abs(region.width) > minViewSize.width &&
                    Math.abs(region.height) > minViewSize.height) {
                root.selected(region)
            } else {
                region = Qt.rect(0, 0, Screen.width, Screen.height)
            }
        }
    }
    
    // decorative tint outside of the frame
    Rectangle {
        anchors.left: parent.left
        anchors.right: frame.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        color: tint
    }
    
    Rectangle {
        anchors.left: frame.right
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        color: tint
    }
    
    Rectangle {
        anchors.left: frame.left
        anchors.right: frame.right
        anchors.top: parent.top
        anchors.bottom: frame.top
        color: tint
    }
    
    Rectangle {
        anchors.left: frame.left
        anchors.right: frame.right
        anchors.top: frame.bottom
        anchors.bottom: parent.bottom
        color: tint
    }
    
    // frame
    Rectangle {
        id: frame
        x: Math.min(region.x, region.x + region.width)
        y: Math.min(region.y, region.y + region.height)
        width: Math.abs(region.width)
        height: Math.abs(region.height)
        border.width: 1
        border.color: borderColor
        color: "#00000000"
        
        Text {
            anchors.left: parent.left
            anchors.bottom: parent.top
            padding: 2
            color: "#ffffffff"
            text: `${frame.width} x ${frame.height}`
        }
    }
}
