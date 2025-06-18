const express = require('express');
const cors = require('cors');
const http = require('http');
const WebSocket = require('ws');
const app = express();
const server = http.createServer(app);

const { mqttClient } = require('./mqtt/client');
const setupMQTTHandlers = require('./mqtt/handlers');
const setupWebSocket = require('./ws/server');
const apiRoutes = require('./routes/api');

// Middleware
app.use(cors());
app.use(express.json());
app.use('/', apiRoutes);

// WebSocket setup
const wss = new WebSocket.Server({ server });
setupWebSocket(wss);

// MQTT setup
setupMQTTHandlers(mqttClient, wss);

// Start server
const PORT = 3001;
server.listen(PORT, () => console.log(`🚀 Server running at http://localhost:${PORT}`));

// Graceful shutdown
process.on('SIGINT', () => {
  console.log('🛑 Shutting down...');
  mqttClient.end();
  require('./db/client').end();
  server.close(() => process.exit(0));
});
