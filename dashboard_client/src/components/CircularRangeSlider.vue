<template>
  <div class="circular-slider" @mousedown="startDrag" @touchstart.prevent="startDrag" ref="slider">
    <svg :width="size" :height="size" :viewBox="`0 0 ${size} ${size}`">
      <!-- Background Circle -->
      <circle
        :cx="center"
        :cy="center"
        :r="radius"
        stroke="#ddd"
        stroke-width="10"
        fill="none"
      />
      <!-- Progress Arc -->
      <path
        :d="arcPath"
        stroke="#3b82f6"
        stroke-width="10"
        fill="none"
        stroke-linecap="round"
      />
      <!-- Knob -->
      <circle
        :cx="knobPos.x"
        :cy="knobPos.y"
        r="15"
        fill="#3b82f6"
        stroke="#fff"
        stroke-width="3"
        cursor="pointer"
      />
    </svg>
    <div class="value-display">{{ value }}%</div>
  </div>
</template>

<script>
export default {
  name: "CircularRangeSlider",
  props: {
    size: {
      type: Number,
      default: 200,
    },
    min: {
      type: Number,
      default: 0,
    },
    max: {
      type: Number,
      default: 100,
    },
    modelValue: {
      type: Number,
      default: 0,
    },
  },
  emits: ["update:modelValue"],
  data() {
    return {
      dragging: false,
      value: this.modelValue,
    };
  },
  computed: {
    center() {
      return this.size / 2;
    },
    radius() {
      return this.center - 20; // padding for knob/stroke
    },
    angle() {
      // Convert current value to angle (0 = -90deg at top, goes clockwise)
      return ((this.value - this.min) / (this.max - this.min)) * 360 - 90;
    },
    knobPos() {
      const rad = (this.angle * Math.PI) / 180;
      return {
        x: this.center + this.radius * Math.cos(rad),
        y: this.center + this.radius * Math.sin(rad),
      };
    },
    arcPath() {
      // SVG arc from startAngle (-90deg) to current angle
      const startAngle = -90;
      const endAngle = this.angle;
      const startRad = (startAngle * Math.PI) / 180;
      const endRad = (endAngle * Math.PI) / 180;

      const startX = this.center + this.radius * Math.cos(startRad);
      const startY = this.center + this.radius * Math.sin(startRad);
      const endX = this.center + this.radius * Math.cos(endRad);
      const endY = this.center + this.radius * Math.sin(endRad);

      const largeArcFlag = endAngle - startAngle <= 180 ? 0 : 1;

      return `
        M ${startX} ${startY}
        A ${this.radius} ${this.radius} 0 ${largeArcFlag} 1 ${endX} ${endY}
      `;
    },
  },
  watch: {
    modelValue(newVal) {
      this.value = newVal;
    },
  },
  methods: {
    updateValueFromPos(x, y) {
      const rect = this.$refs.slider.getBoundingClientRect();
      const cx = rect.left + this.center;
      const cy = rect.top + this.center;
      const dx = x - cx;
      const dy = y - cy;
      let angle = Math.atan2(dy, dx) * (180 / Math.PI);

      angle += 90; // shift so 0 at top
      if (angle < 0) angle += 360;

      const newValue = Math.round(
        (angle / 360) * (this.max - this.min) + this.min
      );
      this.value = Math.min(this.max, Math.max(this.min, newValue));
      this.$emit("update:modelValue", this.value);
    },
    onDrag(e) {
      if (!this.dragging) return;
      let clientX, clientY;
      if (e.touches) {
        clientX = e.touches[0].clientX;
        clientY = e.touches[0].clientY;
      } else {
        clientX = e.clientX;
        clientY = e.clientY;
      }
      this.updateValueFromPos(clientX, clientY);
    },
    stopDrag() {
      this.dragging = false;
      window.removeEventListener("mousemove", this.onDrag);
      window.removeEventListener("mouseup", this.stopDrag);
      window.removeEventListener("touchmove", this.onDrag);
      window.removeEventListener("touchend", this.stopDrag);
    },
    startDrag(e) {
      this.dragging = true;
      this.onDrag(e);
      window.addEventListener("mousemove", this.onDrag);
      window.addEventListener("mouseup", this.stopDrag);
      window.addEventListener("touchmove", this.onDrag);
      window.addEventListener("touchend", this.stopDrag);
    },
  },
};
</script>

<style scoped>
.circular-slider {
  user-select: none;
  width: fit-content;
  margin: auto;
  position: relative;
}
.value-display {
  position: absolute;
  top: 50%;
  left: 50%;
  transform: translate(-50%, -50%);
  font-weight: bold;
  font-size: 1.5em;
  color: #3b82f6;
  pointer-events: none;
  font-family: Arial, sans-serif;
}
</style>
