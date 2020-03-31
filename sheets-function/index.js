//Google API to allow access to sheets
const {google} = require('googleapis');

const sheets = google.sheets({version: 'v4'});

const dateFormat = require('dateformat');

const credentials = require("./credentials.json");

const config = require("./config.json");

exports.sheetsArgon = (event, context) => {
	const pubsubMessage = event.data;

	// console.log("event", event);

	if (!pubsubMessage.data) {
		console.log("no data");
		return;
	}

	var jsonData = JSON.parse(Buffer.from(pubsubMessage.data, 'base64').toString());


	// console.log("jsonData", jsonData);
	var row = [];

	// This generates the columns for the row to add to the spreadsheet
	var d = Date.parse(pubsubMessage.attributes.published_at);
	
	row.push(dateFormat(d, 'm/d/yyyy HH:MM:ss'));
			
	const fields = ['device', 'temperature'];
	fields.forEach(function(field) {
		if (jsonData.hasOwnProperty(field)) {
			row.push(jsonData[field]);
		}
		else {
			row.push('');
		}
	});
	
	// This is the JWT authorization for the spreadsheet
	var jwt = new google.auth.JWT(
			credentials.client_email, null, credentials.private_key,
			['https://www.googleapis.com/auth/spreadsheets']);
	
	sheets.spreadsheets.values.append({
		spreadsheetId: config.spreadsheetId,
		range: 'A1',
		auth: jwt,
		key: config.apiKey,
		valueInputOption: 'USER_ENTERED',
		resource: {values: [row]}
	}, function(err, result) {
		if (err) {
			console.log("Error writing sheet", err);
		}
		else {
			console.log('Updated sheet: ' + result.data.updates.updatedRange, row);
		}
	});

};
