const { dbConfig } = require('../config');
const { Client } = require('pg');

const dbClient = new Client(dbConfig);
dbClient.connect().then(() => console.log('🛢️ Connected to PostgreSQL'));

module.exports = dbClient;
