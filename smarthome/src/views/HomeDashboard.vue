<template>
    <div class="dashboard">
      <h1>Smart Home Dashboard</h1>
      <div class="dashboard-cards">
        <div class="card">
          <h2>Light Control</h2>
          <p>Status: {{ lights }}</p>
        </div>
        <div class="card">
          <h2>Temperature</h2>
          <p>Current: {{ temperature }}</p>
        </div>
        <div class="card">
          <h2>Energy Consumption</h2>
          <p>Usage: {{ energyConsumption }}</p>
        </div>
      </div>
    </div>
  </template>
  
  <script>
export default {
  name: 'HomeDashboard',
  data() {
    return {
      lights: 'ON',
      temperature: '22°C',
      energyConsumption: '150 kWh',
    };
  },
};
</script>
  
  <style scoped>
  .dashboard {
    padding: 20px;
    font-family: Arial, sans-serif;
    background-color: #f9f9f9;
  }
  
  .dashboard-cards {
    display: flex;
    gap: 20px;
  }
  
  .card {
    padding: 15px;
    background-color: #ffffff;
    border: 1px solid #ddd;
    border-radius: 5px;
    box-shadow: 0px 4px 8px rgba(0, 0, 0, 0.1);
    flex: 1;
    text-align: center;
  }
  </style>