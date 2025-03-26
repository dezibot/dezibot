// Slider handling to adjust chartLimit
document.getElementById('slider-input').addEventListener('input', function(event) {
    chartLimit = parseInt(event.target.value);
    console.log(`Chart limit updated to: ${chartLimit}`);
});

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
                    <div class="sensor-value" id="sensor-value-${sensorId}">${JSON.stringify(sensor.value)}</div>
                    <div id="${sensorId}" style="height: 250px; width:90%;"></div>
                `;
                container.appendChild(sensorDiv);

                if (!charts[sensorId]) {
                    createChart(sensorId, sensor.name);
                }
            } else {
                document.getElementById(`sensor-value-${sensorId}`).textContent = JSON.stringify(sensor.value);
            }

            handleIncomingData(sensorId, sensor.value);
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

function createChart(chartId, sensorName) {
    dps[chartId] = [[], [], []]; // Support for up to 3 datasets
    charts[chartId] = new CanvasJS.Chart(chartId, {
        title: {
            text: sensorName
        },
        data: [
            { type: "line", dataPoints: dps[chartId][0], showInLegend: true, name: "x" },
            { type: "line", dataPoints: dps[chartId][1], showInLegend: true, name: "y" },
            { type: "line", dataPoints: dps[chartId][2], showInLegend: true, name: "z" }
        ],
        axisY: {
            title: "Value"
        }
    });
}

function handleIncomingData(chartId, value) {
    if (!charts[chartId]) {
        return;
    }

    const numericValue = parseFloat(value);
    if (!isNaN(numericValue)) {
        dps[chartId][0].push({ x: xVal, y: value });
    } else if (typeof value === 'string' && value.includes(',')) {
        const parts = value.split(',').map(part => parseFloat(part.split(':')[1]));
        if (parts.length >= 2) {
            dps[chartId][0].push({ x: xVal, y: parts[0] });
            dps[chartId][1].push({ x: xVal, y: parts[1] });
        }
        if (parts.length === 3) {
            dps[chartId][2].push({ x: xVal, y: parts[2] });
        }
    } else {
        console.warn(`Unknown data format:`, value);
        return;
    }

    // Shift data if limit exceeded
    for (let i = 0; i < dps[chartId].length; i++) {
        if (dps[chartId][i].length > chartLimit) {
            dps[chartId][i].shift();
        }
    }

    charts[chartId].render();
}
