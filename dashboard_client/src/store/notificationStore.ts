import { defineStore } from 'pinia';

export const useNotificationStore = defineStore('notificationStore', {
  state: () => ({
    notifications: [] as Array<{ id: number; message: string }>,
  }),
  actions: {
    addNotification(message: string) {
      const newId = this.notifications.length + 1;
      this.notifications.push({ id: newId, message });
    },
    clearNotifications() {
      this.notifications = [];
    },
  },
});