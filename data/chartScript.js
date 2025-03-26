// Slider handling to adjust chartLimit
const sliderInput = document.getElementById('slider-input');

function handleSliderInput(event) {
    chartLimit = parseInt(event.target.value, 10);
    console.log(`Chart limit updated to: ${chartLimit}`);
}

sliderInput.addEventListener('input', handleSliderInput);

// Chart management
const charts = {};
const dataPoints = {};
let chartLimit = 100;
let xVal = 0;

// Data fetching and DOM update
async function fetchSensorData() {
    try {
        const response = await fetch('/getEnabledSensorValues');
        if (!response.ok) throw new Error('Failed to fetch data');
        const sensors = await response.json();
        const container = document.getElementById('sensor-container');



        sensors.forEach((sensor, index) => {
            const sensorId = `chartContainer-${index}`;
            const valueElementId = `sensor-value-${sensorId}`;

            if (!document.getElementById(sensorId)) {
                const sensorDiv = document.createElement('div');
                sensorDiv.className = 'sensor';
                sensorDiv.innerHTML = `
                    <div>${sensor.name}</div>
                    <div class="sensor-value" id="${valueElementId}">${JSON.stringify(sensor.value)}</div>
                    <div id="${sensorId}" style="height: 250px; width:90%;"></div>
                `;
                container.appendChild(sensorDiv);

                if (!charts[sensorId]) {
                    createChart(sensorId, sensor.name);
                }
            } else {
                const valueElement = document.getElementById(valueElementId);
                if (valueElement) {
                    valueElement.textContent = JSON.stringify(sensor.value);
                }
            }

            handleIncomingData(sensorId, sensor.value);
        });

        xVal++;
    } catch (error) {
        console.error('Error fetching sensor data:', error);
    }
}

// Chart creation
function createChart(chartId, sensorName) {
    dataPoints[chartId] = [[], [], []];
    charts[chartId] = new CanvasJS.Chart(chartId, {
        // title: { text: sensorName },
        axisY: { title: "Value" },
        data: [
            createDataSeries("x", dataPoints[chartId][0]),
            createDataSeries("y", dataPoints[chartId][1]),
            createDataSeries("z", dataPoints[chartId][2])
        ],
        // TODO: we could style this, I'll leave it like this for now
        backgroundColor: "transparent"
    });
}

// Data series factory function
function createDataSeries(name, points) {
    return {
        type: "line",
        dataPoints: points,
        showInLegend: true,
        name: name
    };
}

// Data processing and chart update
function handleIncomingData(chartId, value) {
    if (!charts[chartId]) return;

    const numericValue = parseFloat(value);

    if (!isNaN(numericValue)) {
        dataPoints[chartId][0].push({ x: xVal, y: numericValue });
    } else if (typeof value === 'string' && value.includes(',')) {
        const parts = value.split(',')
            .map(part => parseFloat(part.split(':')[1]))
            .filter(num => !isNaN(num));

        parts.forEach((part, index) => {
            if (index < 3 && dataPoints[chartId][index]) {
                dataPoints[chartId][index].push({ x: xVal, y: part });
            }
        });
    } else {
        console.warn('Unknown data format:', value);
        return;
    }

    // Maintain data point limits
    dataPoints[chartId].forEach(dataSeries => {
        if (dataSeries.length > chartLimit) {
            dataSeries.shift();
        }
    });

    charts[chartId].render();
}

// Start periodic updates
setInterval(fetchSensorData, 100);
fetchSensorData();