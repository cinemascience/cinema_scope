/**
* This script navigates through the qt installer for Linux and Mac, selecting the correct
* tools to download and install.
*
* The only difference between this script and the windows one is that this one
* only downloads the installer framework, because we can get the rest of qt
* through apt/brew
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
