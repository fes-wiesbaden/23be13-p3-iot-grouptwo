const dbClient = require('./client');

function insertSensorData(temp, temp_value) {
  return dbClient.query('INSERT INTO sensors(temp, temp_value) VALUES ($1, $2)', [temp, temp_value]);
}

function getSensorData() {
  return dbClient.query('SELECT * FROM sensors ORDER BY timestamp DESC LIMIT 10');
}

module.exports = { insertSensorData, getSensorData };
