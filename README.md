# MQ135-data-collection-in-google-sheet:

Connect MQ135 Analog output pin to A0 on nodemcu. Run this code on google sheet scripts and publish as web app. Remember to use ESP8266 version 2.4.2 on Arduino IDE.

Google Script code:

function doGet(e){
  var mo = e.parameter.func;
  if(mo == "addData"){
    var stat = add_data(e);
    if(stat == 1){
      var result = {
        status: true
      };
    return ContentService.createTextOutput(JSON.stringify(result))
    .setMimeType(ContentService.MimeType.JSON);
    }
  }
}

function add_data(e){
  var sheet = SpreadsheetApp.openByUrl('https://docs.google.com/spreadsheets/d/1Pz8s5PqW2e_q53a8e5-t4PYw3z7LHOuX1EKFfXt27OM/edit?usp=sharing');
  
  var lastVal = sheet.getRange("A1:A").getValues();
  var id = lastVal.filter(String).length;
  
  var CurrentDate = new Date() ;  
  var Date_ = Utilities.formatDate(CurrentDate, "BST", "dd/MM/yyyy") ;
  var Time_ = Utilities.formatDate(CurrentDate, "BST", "HH:mm:ss") ;
 
  sheet.appendRow([id,Date_,Time_,e.parameter.val]);  


  return 1;
}
