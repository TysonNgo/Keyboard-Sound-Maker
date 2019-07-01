# Enable mod-script-pipe in Audacity
# Edit > Preferences > Modules
# Restart Audacity, then run this script.


import os
import sys
from PyQt5.QtWidgets import (
    QApplication, QHBoxLayout, QVBoxLayout,
    QWidget, QLabel, QPushButton, QSlider, QTextEdit,
    QFileDialog, QMessageBox
)
from PyQt5.QtCore import Qt
from audacity_script_pipe import do
#def do(s):
#    pass

class QWrapLabel(QLabel):
    def __init__(self, text, parent=None):
        super().__init__(text, parent)
        self.setWordWrap(True)
        self.setTextInteractionFlags(Qt.TextSelectableByMouse)

class GUI(QWidget):
    def __init__(self):
        super().__init__()
        self.sil_lev = 50.0
        self.sil_dur = 0.1
        self.label_before_dur = 0.0
        self.label_after_dur = 0.1

        self.sil_lev_text = None
        self.sil_lev_slider = None

        self.initUI()

    def initUI(self):
        vbox = QVBoxLayout()
        self.setLayout(vbox)
        self.setStyleSheet('font: 12pt Helvetica')

        noise_reduction = QPushButton('Pop up Noise Reduction dialog', self)
        noise_reduction.clicked.connect(self.popup_noise_reduction)
        
        # Audacity 2.3.2 scripting does not appear to allow
        # arguments to be passed to SoundFinder, however, it
        # will still run SoundFinder, but it will use the setting
        # that the user currently has it set to.

        # sil_lev_container = QWidget()
        # sil_lev_hbox = QHBoxLayout()
        # sil_lev_container.setLayout(sil_lev_hbox)
        # self.sil_lev_text = QTextEdit()
        # self.sil_lev_text.setReadOnly(True)
        # self.sil_lev_slider = QSlider(Qt.Horizontal)
        # self.sil_lev_slider.setMinimum(0)
        # self.sil_lev_slider.setMaximum(100)
        # self.sil_lev_text.setText(f'Treat audio below this level as silence [ -dB]: {self.sil_lev}')
        # self.sil_lev_slider.setValue(self.sil_lev)
        # self.sil_lev_slider.setTickPosition(QSlider.TicksBelow)
        # self.sil_lev_slider.setTickInterval(1)
        # self.sil_lev_slider.valueChanged.connect(self.set_sil_lev)
        # sil_lev_hbox.addWidget(self.sil_lev_text)
        # sil_lev_hbox.addWidget(self.sil_lev_slider)
        sound_finder = QPushButton('Execute Sound Finder', self)
        sound_finder.clicked.connect(self.exec_sound_finder)

        rename_labels = QPushButton('Rename the track labels', self)
        rename_labels.clicked.connect(self.exec_rename_labels)

        export_multiple = QPushButton('Pop up Export Multiple dialog', self)
        export_multiple.clicked.connect(self.popup_export_multiple)

        separate_files = QPushButton('Separate Files', self)
        separate_files.clicked.connect(self.exec_separate_files)

        vbox.addWidget(QWrapLabel('1. Record yourself pressing and releasing keys on the keyboard. Be sure to include a delay between when you press and release a key, because you want to isolate each key press/release sound.', self))
        vbox.addWidget(QWrapLabel('2. Manually apply the Noise Reduction effect by first, getting the noise profile (there is a button for this in the pop up dialog) of some selected white noise in the recording, then run the tool.', self))
        vbox.addWidget(noise_reduction)
        vbox.addWidget(QWrapLabel('3. Manually trim the start and end of the audio.', self))
        vbox.addWidget(QWrapLabel('4. Apply Sound Finder, to produce track labels for the key press/release sounds.', self))
        # vbox.addWidget(sil_lev_container)
        vbox.addWidget(sound_finder)
        vbox.addWidget(QWrapLabel('5. Remove junk track labels that do not contain a key press/release sound. The key sounds associated with the track labels should now be in the repeating order of [press, release, ..., press, release].', self))
        vbox.addWidget(QWrapLabel('6. Rename the track labels', self))
        vbox.addWidget(rename_labels)
        vbox.addWidget(QWrapLabel('7. Track labels should now be properly named [keypress_0, keyrelease_0, keypress_1, keyrelease_1, ...]. You are now ready to export.', self))
        vbox.addWidget(export_multiple)
        vbox.addWidget(QWrapLabel('8. Execute key press/release audio file separator script—creates directories for press/release and moves the appropriate file(s) to the respective directory.', self))
        vbox.addWidget(separate_files)

        self.resize(800, 250)
        self.move(300, 300)
        self.setWindowTitle('Audacity Recording Keyboard Sound Helper')
        self.show()

    def popup_noise_reduction(self):
        do("Noise Reduction...:")

    def set_sil_lev(self):
        self.sil_lev = float(self.sil_lev_slider.value())
        self.sil_lev_text.setText(f'Treat audio below this level as silence [ -dB]: {self.sil_lev}')

    def exec_sound_finder(self):
        do(f"SoundFinder: sil-lev={self.sil_lev} " +
           f"sil-dur={self.sil_dur} " +
           f"labelafterdur={self.label_before_dur} " +
           f"labelafterdur={self.label_after_dur}")

    def exec_rename_labels(self):
        info = do('GetInfo: Type=Labels')
        # response from info should be a JSON in the format:
        # [
        #     [
        #         track_index (int),
        #         [
        #             [
        #                 start (float),
        #                 end (float),
        #                 label_name (str)
        #             ],
        #             ...
        #         ]
        #     ],
        #     ...
        # ]
        m = 0
        for index, labels in info:
            n = -1
            for i in range(len(labels)):
                if not (i & 1):
                    n += 1
                text = 'key' + ('release' if i & 1 else 'press') + '_' + str(n)
                do(f'SetLabel: Label={m+i} Text="{text}"')
            if i == len(labels)-1:
                m += i+1

    def popup_export_multiple(self):
        do("ExportMultiple:")

    def exec_separate_files(self):
        dialog = QFileDialog()
        out = dialog.getExistingDirectory(self, 'Select a directory', '', QFileDialog.ShowDirsOnly)
        if not out:
            return
        press_dir = os.path.join(out, 'keypress')
        release_dir = os.path.join(out, 'keyrelease')

        try:
            os.mkdir(press_dir)
            os.mkdir(release_dir)
        except Exception as ex:
            print(ex)

        files = [f for f in os.listdir(out) if 'keypress_' in f or 'keyrelease_' in f]

        for f in files:
            original = os.path.join(out, f)
            if 'press' in f:
                os.rename(original, os.path.join(press_dir, f))
            elif 'release' in f:
                os.rename(original, os.path.join(release_dir, f))

        done = QMessageBox()
        done.setText(f'Files in "{out}" have been separated')
        done.exec_()


if __name__ == '__main__':
    app = QApplication(sys.argv)
    gui = GUI()
    sys.exit(app.exec_())
