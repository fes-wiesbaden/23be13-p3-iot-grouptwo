<template>
  <div>
    <CircularRangeSlider v-model="fanSpeed" />
    <p>Fan Speed: {{ fanSpeed }}%</p>
  </div>
</template>

<script>
import CircularRangeSlider  from '@/components/CircularRangeSlider.vue';

export default {
  components: { CircularRangeSlider },
  data() {
    return {
      fanSpeed: 30,
    };
  },
};
</script>
