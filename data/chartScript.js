// Setup for data fetching
async function fetchSensorData() {
    try {
        const response = await fetch('/getEnabledSensorValues');
        if (!response.ok) throw new Error('Failed to fetch data');
        const sensors = await response.json();
        const container = document.getElementById('sensor-container');
        container.innerHTML = '';
        for (const sensor of sensors) {
            if (sensor.name.includes('Ambient')) {
                updateChart(sensor.value);
            }
            const sensorDiv = document.createElement('div');
            sensorDiv.className = 'sensor';
            sensorDiv.innerHTML = `
            <div>${sensor.name}</div>
            <div class="sensor-value">${sensor.value}</div>
            <div id="chartContainer" style="height: 370px; width:100%;"></div>
          `;
            container.appendChild(sensorDiv);
        }
    } catch (error) {
        console.error('Error fetching sensor data:', error);
    }
}
setInterval(fetchSensorData, 100);
fetchSensorData();

// Setup for ambient light chart

let dps = [];
let chart;
let xVal = 0;
let chartLimit = 100;

window.onload = function() {
    chart = new CanvasJS.Chart("chartContainer", {
        title: {
            text: "Ambient Light"
        },
        data: [{
            type: "line",
            dataPoints: dps
        }],
        axisY: {
            title: "Value"
        }
    });
}

// INFO: for single value charts
function updateChart(value) {
    // check if the chart exists
    if (!chart) {
        return;
    }
    // add the value
    dps.push({
        x: xVal,
        y: value
    });
    xVal++;
    if (dps.length > chartLimit) {
        dps.shift();
    }
    chart.render();
}