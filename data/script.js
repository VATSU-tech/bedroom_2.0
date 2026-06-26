/**
 * SmartSchool Web Interface Client Script
 */

// UI Elements
let connectionStatusEl;
let ldrValueEl;

// Document Ready Setup
document.addEventListener('DOMContentLoaded', () => {
    connectionStatusEl = document.getElementById("connectionStatus");
    ldrValueEl = document.getElementById("valeurLuminosite");

    // Run initial update and set interval
    updateLuminosity();
    setInterval(updateLuminosity, 2000);
});

// Update connection status helper
function setConnected(connected) {
    if (!connectionStatusEl) return;
    if (connected) {
        connectionStatusEl.textContent = "Connecté";
        connectionStatusEl.style.backgroundColor = "rgba(16, 185, 129, 0.15)";
        connectionStatusEl.style.color = "#10b981";
        connectionStatusEl.style.borderColor = "rgba(16, 185, 129, 0.3)";
    } else {
        connectionStatusEl.textContent = "Déconnecté";
        connectionStatusEl.style.backgroundColor = "rgba(239, 68, 68, 0.15)";
        connectionStatusEl.style.color = "#ef4444";
        connectionStatusEl.style.borderColor = "rgba(239, 68, 68, 0.3)";
    }
}

// Turn LED ON
async function onButton() {
    try {
        const response = await fetch('/on');
        if (response.ok) {
            const data = await response.json();
            console.log("LED Status:", data.led);
            setConnected(true);
        } else {
            console.error("Failed to turn LED on");
            setConnected(false);
        }
    } catch (error) {
        console.error("Error communicating with server:", error);
        setConnected(false);
    }
}

// Turn LED OFF
async function offButton() {
    try {
        const response = await fetch('/off');
        if (response.ok) {
            const data = await response.json();
            console.log("LED Status:", data.led);
            setConnected(true);
        } else {
            console.error("Failed to turn LED off");
            setConnected(false);
        }
    } catch (error) {
        console.error("Error communicating with server:", error);
        setConnected(false);
    }
}

// Fetch luminosity data from server periodically
async function updateLuminosity() {
    try {
        const response = await fetch('/lireLuminosite');
        if (response.ok) {
            const data = await response.json();
            if (ldrValueEl) {
                ldrValueEl.textContent = data.value;
            }
            setConnected(true);
        } else {
            setConnected(false);
        }
    } catch (error) {
        console.error("Error fetching luminosity:", error);
        setConnected(false);
    }
}