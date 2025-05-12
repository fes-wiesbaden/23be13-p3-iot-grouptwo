<template>
    <div class="card">
      <section class="info-section">
        <div class="background-design">
          <div class="circle"></div>
          <div class="circle"></div>
          <div class="circle"></div>
        </div>
        <div class="left-side">
          <div class="weather">
            <div>
              <svg stroke="#ffffff" fill="#ffffff" xmlns="http://www.w3.org/2000/svg" viewBox="0 0 1024 1024">
                <path
                  d="M512 704a192 192 0 1 0 0-384 192 192 0 0 0 0 384zm0 64a256 256 0 1 1 0-512 256 256 0 0 1 0 512z..."
                  fill="#ffffff"
                ></path>
              </svg>
            </div>
            <div>Sunny</div>
          </div>
          <div class="temperature">36°</div>
          <div class="range">42°/28°</div>
        </div>
        <div class="right-side">
          <div>
            <div class="hour">{{ time }}</div>
            <div class="date">{{ date }}</div>
          </div>
          <div class="city">A Coruña</div>
        </div>
      </section>
  
      <section class="days-section">
        <button v-for="day in days" :key="day.name">
          <span class="day">{{ day.name }}</span>
          <span class="icon-weather-day" v-html="day.icon" />
        </button>
      </section>
    </div>
  </template>
  
  <script setup lang="ts">
  import { ref, onMounted } from 'vue';
  
  const time = ref('');
  const date = ref('');
  const days = [
    {
      name: 'TUE',
      icon: '<svg stroke="#ffffff" fill="#ffffff" viewBox="0 0 1024 1024"><path d="..." /></svg>',
    },
    {
      name: 'WED',
      icon: '<svg stroke="#ffffff" fill="#ffffff" viewBox="0 0 24 24"><path d="..." /></svg>',
    },
    {
      name: 'THU',
      icon: '<svg stroke="#ffffff" fill="#ffffff" viewBox="0 0 24 24"><path d="..." /></svg>',
    },
    {
      name: 'FRI',
      icon: '<svg stroke="#ffffff" fill="#ffffff" viewBox="0 0 1024 1024"><path d="..." /></svg>',
    },
  ];
  
  function updateClock() {
    const now = new Date();
    time.value = now.toLocaleTimeString([], { hour: '2-digit', minute: '2-digit' });
    date.value = now.toDateString().toUpperCase().slice(0, 10);
  }
  
  onMounted(() => {
    updateClock();
    setInterval(updateClock, 1000);
  });
  </script>
  
  <style scoped>
  /* Include all your provided CSS styles here */
  .card {
    display: flex;
    flex-direction: column;
    align-items: center;
    height: 180px;
    width: 280px;
    border-radius: 25px;
    background: lightgrey;
    overflow: hidden;
    transition: 100ms ease;
    box-shadow: rgba(0, 0, 0, 0.15) 2px 3px 4px;
  }
  
  /* All other provided styles below this */
  .info-section {
    position: relative;
    display: flex;
    align-items: center;
    justify-content: space-between;
    width: 100%;
    height: 75%;
    color: white;
  }
  .left-side {
    display: flex;
    flex-direction: column;
    justify-content: space-around;
    height: 100%;
    z-index: 1;
    padding-left: 18px;
  }
  button {
    display: block;
    border: none;
    background: transparent;
  }
  .weather {
    display: flex;
    align-items: center;
    justify-content: flex-start;
    gap: 5px;
  }
  .weather div {
    display: flex;
    align-items: center;
  }
  .weather div:nth-child(1) {
    width: 40%;
    height: auto;
  }
  .temperature {
    font-size: 34pt;
    font-weight: 500;
    line-height: 8%;
  }
  .right-side {
    display: flex;
    flex-direction: column;
    align-items: flex-end;
    justify-content: space-around;
    height: 100%;
    padding-right: 18px;
    z-index: 1;
  }
  .right-side > div {
    display: flex;
    flex-direction: column;
    align-items: flex-end;
  }
  .hour {
    font-size: 19pt;
    line-height: 1em;
  }
  .date {
    font-size: 15px;
  }
  .background-design {
    position: absolute;
    height: 100%;
    width: 100%;
    background-color: #ec7263;
    overflow: hidden;
  }
  .circle {
    background-color: #efc745;
  }
  .circle:nth-child(1) {
    position: absolute;
    top: -80%;
    right: -50%;
    width: 300px;
    height: 300px;
    opacity: 0.4;
    border-radius: 50%;
  }
  .circle:nth-child(2) {
    position: absolute;
    top: -70%;
    right: -30%;
    width: 210px;
    height: 210px;
    opacity: 0.4;
    border-radius: 50%;
  }
  .circle:nth-child(3) {
    position: absolute;
    top: -35%;
    right: -8%;
    width: 100px;
    height: 100px;
    opacity: 1;
    border-radius: 50%;
  }
  .days-section {
    display: flex;
    align-items: center;
    justify-content: space-between;
    width: 100%;
    height: 25%;
    background-color: #974859;
    gap: 2px;
    box-shadow: inset 0px 2px 5px #974859;
  }
  .days-section button {
    display: flex;
    align-items: center;
    justify-content: center;
    height: 100%;
    width: 100%;
    background-color: #a75265;
    box-shadow: inset 0px 2px 5px #974859;
    cursor: pointer;
    transition: 100ms ease;
    gap: 5px;
  }
  .days-section button:hover {
    scale: 0.9;
    border-radius: 10px;
  }
  .days-section .day {
    font-size: 10pt;
    font-weight: 500;
    color: white;
    opacity: 0.7;
  }
  .icon-weather-day {
    display: flex;
    align-items: center;
    width: 20px;
    height: 100%;
  }
  </style>
  