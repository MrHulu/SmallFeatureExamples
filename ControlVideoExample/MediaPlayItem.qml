import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Window 2.15
import QtMultimedia 5.15

Item {
    id: root
    anchors.fill: parent
    
    property int pos: 0
    property alias source: mediaPlayer.source
    
    MediaPlayer {
        id: mediaPlayer
        videoOutput: videoOutput
        autoPlay: true
        autoLoad: true
        loops: MediaPlayer.Infinite // 循环播放
        playbackRate: 0 // 0表示暂停

        // 有些视频的最后几帧可能会有问题，所以提前50ms结束
        function safeSeek(position) {
            let safePosition = Math.min(mediaPlayer.duration - 50, Math.max(0, position))
            mediaPlayer.seek(safePosition)
        }

    }
    Text {
        text: mediaPlayer.errorString
    }

    VideoOutput {
        id: videoOutput
        anchors.top: text.bottom
        anchors.bottom: slider.top
        anchors.left: parent.left
        anchors.right: parent.right
        visible: mediaPlayer.hasVideo
    } 
    Text {
        id: text
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        text: "Duration(%1) Pos(%2)".arg(mediaPlayer.duration).arg(mediaPlayer.position)
        font.pixelSize: 20
    }
        
    Slider {
        id: slider
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        from: 0
        to: 100
        stepSize: 1
        value: root.pos
        property bool seeking: false
        property int lastSeekTime: 0
        onPressedChanged: {
            if (pressed) {
                mediaPlayer.pause();
            } else {
                seeking = true;
                seekTimer.start();
            }
        }
        onMoved: {                
            if (Date.now() - lastSeekTime > 100) {  // 限制seek频率
                lastSeekTime = Date.now();
                var newPosition = mediaPlayer.duration * value / 100 - 1;
                mediaPlayer.safeSeek(newPosition);
            }
        }
    }
    Timer {
        id: seekTimer
        interval: 200  // 200ms 延迟
        repeat: false
        onTriggered: {
            var newPosition = mediaPlayer.duration * slider.value / 100;
            mediaPlayer.safeSeek(newPosition);
            mediaPlayer.play();
            slider.seeking = false;
        }
    }   
    // Connections {
    //     target: mediaPlayer
    //     function positionChanged() {
    //         if (!slider.pressed)
    //             slider.value = mediaPlayer.position * 100 / mediaPlayer.duration
    //         root.pos = slider.value
    //     }
    // }
}