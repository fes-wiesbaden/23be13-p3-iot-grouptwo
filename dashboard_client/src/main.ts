import { createApp } from 'vue';
import { createPinia } from 'pinia'; // Import Pinia
import App from './App.vue';
import router from './router'; // Import Router
import './assets/main.css'
import './assets/tailwind.css';
import '@fortawesome/fontawesome-free/css/all.min.css';



// Create Pinia instance
const pinia = createPinia();

// Create Vue app instance
const app = createApp(App);

// Register Pinia BEFORE mounting the app
app.use(pinia); // Use Pinia as a plugin
app.use(router); // Use Router as a plugin
app.mount('#app'); // Mount app on #app div