import socket
import time
import matplotlib.pyplot as plt

# Parameters
UDP_IP = "192.168.137.130"
UDP_PORT = 1010
latency_data = []  # List to store latency values
timestamps = []  # List to store time for each measurement

# Create a UDP socket
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.settimeout(1)  # Set a timeout for receiving responses (in seconds)

print(f"Sending messages to {UDP_IP}:{UDP_PORT}...")

try:
    while True:
        # Create timestamped message
        start_time = time.time()
        message = f"ping".encode()
        
        # Send the message
        start_time = time.time()
        sock.sendto(message, (UDP_IP, UDP_PORT))
        
        try:
            # Wait for the response
            data, addr = sock.recvfrom(1024)
            end_time = time.time()
        
            rtt = (end_time - start_time) * 1000  # Round-trip time in ms
            
            # Append data to lists
            latency_data.append(rtt)
            timestamps.append(len(latency_data))
            
            print(f"Response from {addr}: RTT: {rtt:.2f} ms")
        
        except socket.timeout:
            print("No response received (Timeout).")
            latency_data.append(None)  # Use None for missing values
            timestamps.append(len(latency_data))
        
        # Plot the data dynamically
        plt.clf()
        plt.plot(timestamps, latency_data, label="Latency (ms)", marker='o')
        plt.xlabel("Packet Number")
        plt.ylabel("Latency (ms)")
        plt.title("UDP Latency Over Time")
        plt.legend()
        plt.grid(True)
        plt.pause(0.1)  # Pause to update the plot
        
        # Short pause before the next message
        time.sleep(0.01)

except KeyboardInterrupt:
    print("\nExiting.")
finally:
    sock.close()

# Show the final plot
plt.show()
