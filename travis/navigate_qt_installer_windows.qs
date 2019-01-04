/**
* This script navigates through the qt installer for Windows, selecting the correct
* tools to download and install.
*
* The only difference between this and the linux/max install scripts is that this
* one downloads mingw64 and qt for mingw64 whereas the other script only installs
* the installer framework.
*
* Thanks to https://github.com/sgsaenger/vipster for the example!
**/

function Controller() {
    installer.autoRejectMessageBoxes();
    installer.installationFinished.connect(function() {
        gui.clickButton(buttons.NextButton);
    })
}

Controller.prototype.WelcomePageCallback = function(){
    gui.clickButton(buttons.NextButton, 3000);
}

Controller.prototype.CredentialsPageCallback = function(){
    gui.clickButton(buttons.NextButton);
}

Controller.prototype.IntroductionPageCallback = function(){
    gui.clickButton(buttons.NextButton);
}

Controller.prototype.TargetDirectoryPageCallback = function(){
    gui.currentPageWidget().TargetDirectoryLineEdit.setText(installer.value("HomeDir") + "/Qt");
    gui.clickButton(buttons.NextButton);
}

Controller.prototype.ComponentSelectionPageCallback = function(){
    var widget = gui.currentPageWidget();
    widget.deselectAll();
    widget.selectComponent("qt.qt5.5120.win64_mingw73");
    widget.selectComponent("qt.tools.win64_mingw730");
    widget.selectComponent("qt.tools.ifw.30")
    gui.clickButton(buttons.NextButton);
}

Controller.prototype.LicenseAgreementPageCallback = function(){
    gui.currentPageWidget().AcceptLicenseRadioButton.setChecked(true);
    gui.clickButton(buttons.NextButton);
}

Controller.prototype.StartMenuDirectoryPageCallback = function(){
    gui.clickButton(buttons.NextButton);
}

Controller.prototype.ReadyForInstallationPageCallback = function(){
    gui.clickButton(buttons.NextButton);
}

Controller.prototype.FinishedPageCallback = function(){
    var checkBoxForm = gui.currentPageWidget().LaunchQtCreatorCheckBoxForm
    if (checkBoxForm && checkBoxForm.launchQtCreatorCheckBox){
        checkBoxForm.launchQtCreatorCheckBox.checked = false;
    }
    gui.clickButton(buttons.FinishButton);
}
