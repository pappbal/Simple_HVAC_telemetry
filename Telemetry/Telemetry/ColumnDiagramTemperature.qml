import QtQuick 2.0

Canvas {
    property var graphVelocities;

    property int columnTemp1: 0;
    property int columnTemp2: 0;
    property int columnTemp3: 0;
    property int columnTemp4: 0;

    function setColumnTemps(newTemp1, newTemp2, newTemp3, newTemp4)
    {
        columnTemp1 = newTemp1;
        columnTemp2 = newTemp2;
        columnTemp3 = newTemp3;
        columnTemp4 = newTemp4;

        return 1987;
    }


    onPaint: {
        var columnHorizontalOffset = 30;
        var labelVerticalOffset = 20;
        var labelHorizontalOffset = 10;
        var verticalScaler = height / 60;
        var columnWidth = width / 8 - 20;

        var context = getContext("2d");
        context.reset();

        // Kitöltés fehérre
        context.fillStyle = "white"
        context.fillRect(0, 0, width, height);

        context.font="normal 12px sans-serif";
        context.strokeText("Temp1", 1 * width / 8 - labelVerticalOffset, height - labelHorizontalOffset);
        context.strokeText("Temp2", 3 * width / 8 - labelVerticalOffset, height - labelHorizontalOffset);
        context.strokeText("Temp3", 5 * width / 8 - labelVerticalOffset, height - labelHorizontalOffset);
        context.strokeText("Temp4", 7 * width / 8 - labelVerticalOffset, height - labelHorizontalOffset);

        context.strokeText(columnTemp1, 1 * width / 8, height - verticalScaler * columnTemp1 - columnHorizontalOffset - labelHorizontalOffset);
        context.strokeText(columnTemp2, 3 * width / 8, height - verticalScaler * columnTemp2 - columnHorizontalOffset - labelHorizontalOffset);
        context.strokeText(columnTemp3, 5 * width / 8, height - verticalScaler * columnTemp3 - columnHorizontalOffset - labelHorizontalOffset);
        context.strokeText(columnTemp4, 7 * width / 8, height - verticalScaler * columnTemp4 - columnHorizontalOffset - labelHorizontalOffset);

        context.beginPath();
        context.lineWidth = 1;
        context.strokeStyle = "black";
        // Mozgás a vonal elejére.
        context.moveTo(0, height - columnHorizontalOffset);
        // Vonal végére mozgás.
        context.lineTo(width, height - columnHorizontalOffset);
        context.closePath();


        context.fillStyle = Qt.rgba(0.254,0.411,0.982,1);
        context.fillRect(0 * width / 4 + (width / 4 - columnWidth) / 2, height - verticalScaler * columnTemp1 - columnHorizontalOffset, columnWidth, verticalScaler * columnTemp1);
        context.fillRect(1 * width / 4 + (width / 4 - columnWidth) / 2, height - verticalScaler * columnTemp2 - columnHorizontalOffset, columnWidth, verticalScaler * columnTemp2);
        context.fillRect(2 * width / 4 + (width / 4 - columnWidth) / 2, height - verticalScaler * columnTemp3 - columnHorizontalOffset, columnWidth, verticalScaler * columnTemp3);
        context.fillRect(3 * width / 4 + (width / 4 - columnWidth) / 2, height - verticalScaler * columnTemp4 - columnHorizontalOffset, columnWidth, verticalScaler * columnTemp4);

        context.stroke();
    } // onPaint vége

    // Vízszintes vonal berajzolása.
    function drawHorizontalLine(context, dataValue, strokeStyle, verticalScaler)
    {
        var offset = height - 20;

        context.beginPath();
        context.lineWidth = 1;
        context.strokeStyle = strokeStyle;
        // Mozgás a vonal elejére.
        context.moveTo(0, offset - verticalScaler * dataValue);
        // Vonal végére mozgás.
        context.lineTo(width, offset - verticalScaler * dataValue);
        context.stroke();
    }

    function drawDataset(context, datarow, strokeStyle, verticalScaler)
    {
        var offset = height - 20;

        context.beginPath();
        context.lineWidth = 3;
        context.strokeStyle = strokeStyle;
        context.moveTo(25, offset-verticalScaler * datarow[0]);
        // A vektoron végigmenve behúzzuk a grafikon szakaszait.
        for(var i=0; i<graphVelocities.length;i++)
        {
            context.lineTo(25+10*i, offset - verticalScaler * datarow[i]);
        }
        context.stroke();
    }
}
