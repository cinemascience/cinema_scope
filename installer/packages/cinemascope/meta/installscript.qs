function Component()
{
    // default constructor
}

Component.prototype.createOperations = function()
{
    // call default implementation to actually install README.txt!
    component.createOperations();

    if (systemInfo.productType === "apple") {
        component.addOperation("CreateShortcut", "@TargetDir@/CinemaScope.app", "@StartMenuDir@/CinemaScope Calculator.lnk",
            "workingDirectory=@TargetDir@");
    }
}
