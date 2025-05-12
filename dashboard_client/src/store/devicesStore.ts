import { defineStore } from 'pinia';

export const useDevicesStore = defineStore('devices', {
  state: () => ({
    devices: [
      { id: 1, name: 'Living Room Light', status: 'off' },
      { id: 2, name: 'Kitchen Light', status: 'on' },
    ],
  }),
  actions: {
    toggleDevice(id: number) {
      const device = this.devices.find((d) => d.id === id);
      if (device) {
        device.status = device.status === 'on' ? 'off' : 'on';
      }
    },
    addDevice(name: string) {
      const newId = this.devices.length + 1;
      this.devices.push({ id: newId, name, status: 'off' });
    },
  },
});