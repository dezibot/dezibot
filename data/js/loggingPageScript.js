const endpointAllLogs = '/logging/getLogs';
const endpointNewLogs = '/logging/getNewLogs';
async function loadLogs(endpoint, logLevel = 'ALL') {
    try {
        // fetch logs from endpoint with current log level
        const response = await fetch(`${endpoint}?level=${logLevel}`);
        if (!response.ok) {
            console.error('Failed to load logs:', response.statusText);
            return;
        }

        const logs = await response.json();
        const tableBody = document.getElementById('log-table-body');

        // clear table content if all logs are read
        if (endpoint === endpointAllLogs) {
            tableBody.innerHTML = '';
        }

        // create a new row for each log entry
        logs.forEach(log => {
            const row = document.createElement('tr');
            row.innerHTML = `
                        <td class=${log.level}>${log.level}</td>
                        <td>${log.timestamp}</td>
                        <td>${log.message}</td>
                    `;
            tableBody.appendChild(row);
        });
    } catch (error) {
        console.error('Error loading logs:', error);
    }
}

// reload logs when log level changes
function onLogLevelChange() {
    const logLevel = document.getElementById('log-level').value;
    loadLogs(endpointAllLogs, logLevel);
}

// manually reload logs when refresh button is clicked
function onRefreshButtonClick() {
    const logLevel = document.getElementById('log-level').value;
    loadLogs(endpointAllLogs, logLevel);
}

// initial load after opening page
window.onload = () => {
    document.getElementById('log-level').value = 'ALL';
    loadLogs(endpointAllLogs);
    setInterval(() => {
        const logLevel = document.getElementById('log-level').value;
        loadLogs(endpointNewLogs, logLevel);
    }, 1000);
};