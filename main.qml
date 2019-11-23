import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.5
import QtQuick.Dialogs 1.2
import Qt.labs.settings 1.1
import io.github.dotbread.byzaster 1.0

Window {
    id: root
    title: qsTr("Byzaster")
    flags: Qt.WindowStaysOnTopHint | Qt.Window | Qt.WindowTitleHint | 
           Qt.CustomizeWindowHint | Qt.WindowCloseButtonHint | 
           Qt.WindowMinimizeButtonHint
    visible: true
    minimumWidth: 160
    maximumWidth: minimumWidth
    minimumHeight: record.height
    maximumHeight: minimumHeight
    
    Settings {
        id: settings
        property alias savePath: saveTo.text
        property alias format: formatOption.currentIndex
        property alias recordCursor: cursorOption.checked
        property alias recordAudio: audioOption.checked
    }
    
    ViewFinder {
        id: finder
        visibility: "Hidden"
        
        onSelected: { 
            hide() 
        }
    }
    
    ByzanzProxy {
        id: byzanz
        region: finder.region
        savePath: settings.savePath
        format: formatModel.get(settings.format).value
    }
    
    Clipboard { 
        id: clipboard
    }
    
    FileDialog {
        id: outputDir
        title: qsTr("Output Directory")
        modality: "WindowModal"
        selectFolder: true
        folder: byzanz.savePath
        
        onAccepted: {
            let url = "" + outputDir.fileUrl
            saveTo.text = url.substring("file://".length)
        }
    }
    
    Column {
        width: parent.width
        height: 300
        spacing: 6
        
        Row {
            Button {
                id: record
                width: 40
                enabled: !byzanz.waiting
                icon.source: byzanz.waiting ? "icons/hourglass.svg" :
                    byzanz.recording ? "icons/stop.svg" : "icons/record.svg"
                icon.width: 24
                icon.height: 24
                highlighted: true
                
                ToolTip {
                    clip: false
                    visible: parent.hovered
                    text: (byzanz.recording ? "Stop" : "Start") + " recording"
                    delay: 1000
                }
        
                onClicked: { 
                    if (byzanz.recording)
                        byzanz.stopRecording()
                    else
                        byzanz.startRecording()
                }
            }
            
            Button {
                id: pickRegion
                enabled: !byzanz.recording
                width: 40
                icon.source: "icons/picture_in_picture.svg"
                icon.width: 24
                icon.height: 24
                
                ToolTip {
                    clip: false
                    visible: parent.hovered
                    text: "Choose a part of the screen to record"
                    delay: 1000
                }
                
                onClicked: { 
                    finder.showFullScreen() 
                }
            }
            
            Button {
                id: copy
                width: 40
                enabled: !byzanz.recording && byzanz.filename != ""
                icon.source: "icons/file_copy.svg"
                icon.width: 24
                icon.height: 24
                
                ToolTip {
                    clip: false
                    visible: parent.hovered
                    text: "Copy recorded file"
                    delay: 1000
                }
                
                onClicked: {
                    if (byzanz.filename !== "") {
                        clipboard.copy(byzanz.filename)
                    }
                }
            }
            
            Button {
                id: showmore
                width: 40
                icon.source: "icons/more_horiz.svg"
                icon.width: 24
                icon.height: 24
                
                ToolTip {
                    clip: false
                    visible: parent.hovered
                    text: "Options"
                    delay: 1000
                }
                
                onClicked: {
                    if (root.height == record.height) {
                        root.minimumHeight = audioOption.y + audioOption.height
                    }
                    else {
                        root.minimumHeight = record.height
                    }
                }
            }
        }
        
        Column {
            width: parent.width
            
            Text {
                text: "Output Dir"
            }
            
            Row {
                width: parent.width
                height: 40
                
                TextField {
                    id: saveTo
                    text: "/tmp"
                    width: parent.width - 30
                }
                
                Button {
                    text: "..."
                    width: 30
                    onClicked: { outputDir.open() }
                }
            }
        }
        
        Column {
            width: parent.width
            
            Text {
                text: "Format"
            }
            
            ComboBox {
                id: formatOption
                width: parent.width
                textRole: "text"
                model: ListModel {
                    id: formatModel
                    ListElement { text: "GIF"; value: ByzanzProxy.FORMAT_GIF }
                    ListElement { text: "FLV"; value: ByzanzProxy.FORMAT_FLV }
                    ListElement { text: "WEBM"; value: ByzanzProxy.FORMAT_WEBM }
                }
            }
        }
        
        CheckBox {
            id: cursorOption
            text: "Record Cursor"
            checked: false
        }
        
        CheckBox {
            id: audioOption
            text: "Record Audio"
            enabled: formatModel.get(settings.format).value !== 
                        ByzanzProxy.FORMAT_GIF
            checked: false
        }
    }
}
