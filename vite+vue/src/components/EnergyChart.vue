<template>
    <div>
      <BarChart :chart-data="chartData" />
    </div>
  </template>
  
  <script lang="ts">
  import { defineComponent } from 'vue';
  import { Bar } from 'vue-chartjs';
  import { Chart as ChartJS, Title, Tooltip, Legend, BarElement } from 'chart.js';
  
  ChartJS.register(Title, Tooltip, Legend, BarElement);
  
  export default defineComponent({
    name: 'EnergyChart',
    components: { BarChart: Bar },
    data() {
      return {
        chartData: {
          labels: ['Living Room', 'Kitchen', 'Bedroom'],
          datasets: [{
            label: 'Energy Usage (kWh)',
            data: [10, 20, 15],
            backgroundColor: ['red', 'blue', 'green'],
          }],
        },
      };
    },
  });
  </script>