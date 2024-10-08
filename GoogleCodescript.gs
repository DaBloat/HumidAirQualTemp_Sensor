function doGet(e) { 
  Logger.log(JSON.stringify(e));
  var result = 'Ok';
  if (e.parameter == 'undefined') {
    result = 'No Parameters';
  }
  else {
    var sheet_id = '1gKiLc9Be0Z5qFa9DcgpEqiYBjsjfPEiOZh544urVDok'; 	// Spreadsheet ID.
    var sheet_name = "DaData";  // Sheet Name in Google Sheets.

    var sheet_open = SpreadsheetApp.openById(sheet_id);
    var sheet_target = sheet_open.getSheetByName(sheet_name);

    var newRow = sheet_target.getLastRow() + 1;

    var rowDataLog = [];

    var Curr_Date = Utilities.formatDate(new Date(), "Asia/Manila", 'dd/MM/yyyy');
    rowDataLog[0] = Curr_Date;  // Date will be written in column A (in the "DHT11 Sensor Data Logger" section).

    var Curr_Time = Utilities.formatDate(new Date(), "Asia/Manila", 'HH:mm:ss');
    rowDataLog[1] = Curr_Time;  // Time will be written in column B (in the "DHT11 Sensor Data Logger" section).

    var sts_val = '';

    for (var param in e.parameter) {
      Logger.log('In for loop, param=' + param);
      var value = stripQuotes(e.parameter[param]);
      Logger.log(param + ':' + e.parameter[param]);
      switch (param) {
        case 'sts':
          sts_val = value;
          break;

        case 'temp':
          rowDataLog[2] = value;  // The temperature value will be written in column D (in the "DHT11 Sensor Data Logger" section).
          result += ', Temperature Written on column C';
          break;

        case 'humd':
          rowDataLog[3] = value; // The humidity value will be written in column E (in the "DHT11 Sensor Data Logger" section).
          result += ', Humidity Written on column D';
          break;

        case 'quality':
          rowDataLog[4] = value;  // The state of Switch_1 will be written in column F (in the "DHT11 Sensor Data Logger" section).
          result += ', Air quality Written on column E';
          break;

        default:
          result += ", unsupported parameter";
      }
    }
    
    // Conditions for writing data received from ESP32 to Google Sheets.
    if (sts_val == 'write') {
      // Writes data to the "DHT11 Sensor Data Logger" section.
      Logger.log(JSON.stringify(rowDataLog));
      var newRangeDataLog = sheet_target.getRange(newRow, 1, 1, rowDataLog.length);
      newRangeDataLog.setValues([rowDataLog]);
      

      return ContentService.createTextOutput(result);
    }
    
  }
}
function stripQuotes( value ) {
  return value.replace(/^["']|['"]$/g, "");
}
//__________________________________________
