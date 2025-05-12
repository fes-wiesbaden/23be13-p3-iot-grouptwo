export const categories = [
    {
      name: 'Living Room',
      devices: [
        { id: 1, name: 'AC', type: 'AC', icon: '🌬️', status: 'off' },
        { id: 2, name: 'Light 1', type: 'Light', icon: '💡', status: 'off' },
        { id: 3, name: 'Light 2', type: 'Light', icon: '💡', status: 'on' }
      ]
    },
    {
      name: 'Kitchen',
      devices: [
        { id: 4, name: 'Oven', type: 'Oven', icon: '🔥', status: 'off' },
        { id: 5, name: 'Refrigerator', type: 'Fridge', icon: '🧊', status: 'on' },
        { id: 6, name: 'Kitchen Light', type: 'Light', icon: '💡', status: 'off' }
      ]
    },
    {
      name: 'Entrance',
      devices: [
        {
          id: 7,
          name: 'Front Door Light',
          type: 'Light',
          icon: '💡',
          status: 'off'
        },
        {
          id: 8,
          name: 'Main Door Camera',
          type: 'Camera',
          icon: '📷',
          status: 'on',
          thumbnailUrl: 'https://loremflickr.com/160/100/camera',
          fullImageUrl: 'https://loremflickr.com/800/600/camera'
        }
      ]
    }
  ];
  