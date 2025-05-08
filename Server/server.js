const express = require('express');
const cors = require('cors');
const { SerialPort } = require('serialport');

// Configuration
const portName = 'COM5'; // ⚠️ Change this to match your system
const baudRate = 9600;

// Create Express app
const app = express();
app.use(cors());
app.use(express.json());

// Open serial connection
const arduino = new SerialPort({ path: portName, baudRate });

arduino.on('open', () => {
  console.log(`✅ Serial port ${portName} opened at ${baudRate} baud`);
});

arduino.on('error', err => {
  console.error('❌ Serial port error:', err.message);
});

// Map light names to command codes
const lightCommandMap = {
  'Front Door Light': '1',
  'Kitchen Light': '2',
  'Light 1': '3',
  'Light 2': '4'
  // ➕ Add more lights and codes here
};

// API to toggle light
app.post('/light', (req, res) => {
  const { name, status } = req.body;
  const baseCmd = lightCommandMap[name];

  if (!baseCmd) {
    return res.status(400).send(`Unknown light device: ${name}`);
  }

  const cmd = status === 'on' ? baseCmd + '1' : baseCmd + '0';

  console.log(`📡 Sending to Arduino: ${cmd} for ${name}`);

  arduino.write(cmd, err => {
    if (err) {
      console.error('⚠️ Failed to write to serial port:', err.message);
      return res.status(500).send('Failed to send command to Arduino');
    }
    res.send(`${name} turned ${status}`);
  });
});

// Start server
const PORT = 3001;
app.listen(PORT, () => {
  console.log(`🚀 Server running at http://localhost:${PORT}`);
});
