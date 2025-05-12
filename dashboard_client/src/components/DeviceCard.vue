<template>
  <div :class="['lamp-card', isOn ? 'active' : 'inactive']">
    <div class="header">
      <span class="status-text">{{ isOn ? 'On' : 'Off' }}</span>
      <label class="switch">
        <input type="checkbox" :checked="isOn" @change="$emit('toggle', !isOn)" />
        <span class="slider"></span>
      </label>
    </div>
    <div class="icon">{{ icon }}</div>
    <h3 class="name">{{ name }}</h3>
    <p class="subtext">{{ isOn ? 'Active' : 'Inactive' }}</p>
  </div>
</template>

<script>
export default {
  name: 'DeviceCard',
  props: {
    name: String,
    type: String,
    isOn: Boolean
  },
  computed: {
    icon() {
      const icons = {
        lamp: '💡',
        fan: '🌀',
        ac: '❄️',
        tv: '📺'
      };
      return icons[this.type] || '🔌';
    }
  }
};
</script>


<style scoped>
.lamp-card {
  width: 220px;
  padding: 16px;
  border-radius: 16px;
  background: #fff;
  color: #333;
  box-shadow: 0 4px 8px rgba(0, 0, 0, 0.06);
  display: flex;
  flex-direction: column;
  align-items: flex-start;
  transition: background 0.3s, color 0.3s;
  margin: 11px;
}

.lamp-card.active {
  background: linear-gradient(135deg, #56cc9d, #38d39f);
  color: white;
}

.header {
  width: 100%;
  display: flex;
  justify-content: space-between;
  align-items: center;
}

.status-text {
  font-size: 14px;
  font-weight: bold;
}

.icon {
  font-size: 36px;
  margin: 20px 0 10px;
}

.name {
  font-size: 16px;
  font-weight: bold;
  margin: 0;
}

.subtext {
  font-size: 13px;
  color: inherit;
}

/* Switch styling */
.switch {
  --secondary-container: #3a4b39;
  --primary: #84da89;
  font-size: 17px;
  position: relative;
  display: inline-block;
  width: 3.7em;
  height: 1.8em;
}

.switch input {
  display: none;
}

.slider {
  position: absolute;
  cursor: pointer;
  top: 0;
  left: 0;
  right: 0;
  bottom: 0;
  background-color: #313033;
  transition: 0.2s;
  border-radius: 30px;
}

.slider:before {
  position: absolute;
  content: "";
  height: 1.4em;
  width: 1.4em;
  border-radius: 20px;
  left: 0.2em;
  bottom: 0.2em;
  background-color: #aeaaae;
  transition: 0.4s;
}

input:checked + .slider {
  background-color: var(--secondary-container);
}

input:checked + .slider:before {
  background-color: var(--primary);
  transform: translateX(1.9em);
}
</style>
