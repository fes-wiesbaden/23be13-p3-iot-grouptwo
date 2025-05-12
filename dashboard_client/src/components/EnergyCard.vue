<template>
  <div class="electricity-card">
    <!-- <div class="header">Consumption by room</div> -->
    <div class="content">
      <div class="chart">
        <Doughnut :data="chartData" :options="chartOptions" />
        <div class="center-label">
          <div class="value">471.3</div>
          <div class="unit">WATTS</div>
        </div>
      </div>
      <div class="legend">
        <div
          v-for="(room, i) in rooms"
          :key="room.name"
          class="legend-item"
        >
          <span
            class="legend-color"
            :style="{ backgroundColor: room.color }"
          ></span>
          <span class="legend-name">{{ room.name }}</span>
          <span class="legend-percent">{{ room.percent }}%</span>
        </div>
      </div>
    </div>
  </div>
</template>

<script>
import { Doughnut } from 'vue-chartjs';
import {
  Chart as ChartJS,
  Title,
  Tooltip,
  Legend,
  ArcElement,
} from 'chart.js';

ChartJS.register(Title, Tooltip, Legend, ArcElement);

export default {
  name: 'ElectricityCard',
  components: {
    Doughnut,
  },
  data() {
    return {
      rooms: [
        { name: 'Living Room', percent: 15, color: '#d4145a' },
        { name: 'Kitchen', percent: 20, color: '#8796a5' },
        { name: 'Attic', percent: 13, color: '#03c0f4' },
        { name: 'Garage', percent: 32, color: '#7de33e' },
        { name: 'Basement', percent: 20, color: '#f9a825' },
      ],
    };
  },
  computed: {
    chartData() {
      return {
        labels: this.rooms.map((room) => room.name),
        datasets: [
          {
            data: this.rooms.map((room) => room.percent),
            backgroundColor: this.rooms.map((room) => room.color),
            borderWidth: 0,
            cutout: '70%',
          },
        ],
      };
    },
    chartOptions() {
      return {
        responsive: true,
        maintainAspectRatio: false,
        plugins: {
          legend: { display: false },
        },
      };
    },
  },
};
</script>

<style scoped>
.electricity-card {
  background: #fff;
  border-radius: 16px;
  padding: 16px;
  box-shadow: 0 4px 12px rgba(0, 0, 0, 0.06);
  height: 100%;
  box-sizing: border-box;
  display: flex;
  flex-direction: column;
}

.header {
  font-size: 18px;
  font-weight: 600;
  color: #1e293b;
  margin-bottom: 16px;
}

.content {
  display: flex;
  align-items: center;
  height: 100%;
}

.chart {
  position: relative;
  width: 45%;
  height: 140px;
}

.center-label {
  position: absolute;
  top: 50%;
  left: 50%;
  transform: translate(-50%, -50%);
  text-align: center;
}

.center-label .value {
  font-size: 24px;
  font-weight: 700;
  color: #1e293b;
}

.center-label .unit {
  font-size: 12px;
  color: #64748b;
}

.legend {
  width: 55%;
  padding-left: 16px;
  display: flex;
  flex-direction: column;
  justify-content: space-between;
}

.legend-item {
  display: flex;
  align-items: center;
  justify-content: space-between;
  font-size: 14px;
  margin-bottom: 8px;
}

.legend-color {
  width: 14px;
  height: 14px;
  border-radius: 4px;
  margin-right: 8px;
  flex-shrink: 0;
}

.legend-name {
  flex-grow: 1;
  color: #1e293b;
  font-weight: 500;
}

.legend-percent {
  color: #64748b;
  font-weight: 500;
}
</style>
