async function loadSensorSettings() {
    try {
        const response = await fetch('/settings/getSensorData');
        if (!response.ok) {
            console.error('Failed to load sensor data:', response.statusText);
            return;
        }

        const sensorData = await response.json();
        const container = document.getElementById('sensor-container');
        container.innerHTML = '';

        // Render sensors and their functions dynamically
        sensorData.forEach(sensor => {
            // Add Sensor Header
            const sensorHeader = document.createElement('h2');
            sensorHeader.textContent = sensor.sensorName;
            container.appendChild(sensorHeader);

            // Add each sensor function
            sensor.functions.forEach(func => {
                const functionDiv = document.createElement('div');
                functionDiv.className = 'sensor-item';

                const label = document.createElement('label');
                label.textContent = func.name;
                functionDiv.appendChild(label);

                const switchLabel = document.createElement('label');
                switchLabel.className = 'switch';

                const input = document.createElement('input');
                input.type = 'checkbox';
                input.checked = func.state;
                input.onchange = () => toggleSensor(func.name, input.checked);
                switchLabel.appendChild(input);

                const slider = document.createElement('span');
                slider.className = 'slider';
                switchLabel.appendChild(slider);

                functionDiv.appendChild(switchLabel);
                container.appendChild(functionDiv);
            });
        });
    } catch (error) {
        console.error('Error loading sensor data:', error);
    }
}

async function toggleSensor(sensorFunction, isEnabled) {
    try {
        const response = await fetch('/settings/toggleFunction', {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify({
                sensorFunction: sensorFunction,
                enabled: isEnabled
            })
        });

        if (!response.ok) {
            console.error(`Failed to toggle sensor function ${sensorFunction}:`, await response.text());
        }
    } catch (error) {
        console.error(`Error toggling sensor function ${sensorFunction}:`, error);
    }
}

// Initial load after opening page
window.onload = loadSensorSettings;