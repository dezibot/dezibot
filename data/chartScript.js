// Setup for data fetching
async function fetchSensorData() {
    try {
        const response = await fetch('/getEnabledSensorValues');
        if (!response.ok) throw new Error('Failed to fetch data');
        const sensors = await response.json();
        const container = document.getElementById('sensor-container');

        let sensorCounter = 0;

        for (const sensor of sensors) {
            const sensorId = `chartContainer-${sensorCounter}`;

            if (!document.getElementById(sensorId)) {
                const sensorDiv = document.createElement('div');
                sensorDiv.className = 'sensor';
                sensorDiv.innerHTML = `
                    <div>${sensor.name}</div>
                    <div class="sensor-value" id="sensor-value-${sensorId}">${sensor.value}</div>
                    <div id="${sensorId}" style="height: 250px; width:90%;"></div>
                `;
                container.appendChild(sensorDiv);

                if (!charts[sensorId]) {
                    createChart(sensorId, sensor.name);
                }
            } else {
                // Update sensor value display
                document.getElementById(`sensor-value-${sensorId}`).textContent = sensor.value;
            }

            let value = parseFloat(sensor.value);
            if (isNaN(value)) {
                // Placeholder for non-numeric values
                // console.warn(`Non-numeric value received for ${sensor.name}, using random placeholder.`);
                value = Math.random() * 100;
            }

            updateChart(sensorId, sensor.value);
            sensorCounter++;
        }
        xVal++;
    } catch (error) {
        console.error('Error fetching sensor data:', error);
    }
}

setInterval(fetchSensorData, 100);
fetchSensorData();

// Chart management
let charts = {};
let dps = {};
let chartLimit = 100;
let xVal = 0;

// TODO: specific arrays for some charts
// Acceleration
let xAcc = [];
let yAcc = [];
let zAcc = [];

// Rotation
let xRot = [];
let yRot = [];
let zRot = [];

// Tilt
let xTil = [];
let yTil = [];

function createChart(chartId, sensorName) {
    dps[chartId] = [];
    charts[chartId] = new CanvasJS.Chart(chartId, {
        title: {
            text: sensorName
        },
        data: [{
            type: "line",
            dataPoints: dps[chartId]
        }],
        axisY: {
            title: "Value"
        }
    });
}

function updateChart(chartId, value) {
    if (!charts[chartId]) {
        return;
    }

    if (chartId === 'chartContainer-0') {
        console.log('Pushing value: ' + value + ' at length ' + xVal);
    }
    dps[chartId].push({
        x: xVal,
        y: value
    });

    if (dps[chartId].length > chartLimit) {
        dps[chartId].shift();
    }

    if (chartId ==='chartContainer-0') {
        console.log(dps[chartId]);
    }

    charts[chartId].render();
}
