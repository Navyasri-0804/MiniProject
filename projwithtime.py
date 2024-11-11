import datetime
import requests
import numpy as np
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import StandardScaler
from sklearn.ensemble import RandomForestClassifier
from sklearn.metrics import accuracy_score

# Function to retrieve data from ThingSpeak
def retrieve_data(channel_id, read_api_key, num_results):
    url = f'https://api.thingspeak.com/channels/{channel_id}/feeds.json?api_key={read_api_key}&results={num_results}'
    response = requests.get(url)
    if response.status_code == 200
        data = response.json()
        entries = data['feeds']
        features = []
        labels = []
        for entry in entries:
            # Extract relevant features and labels from ThingSpeak data
            temperature = float(entry['field1'])
            humidity = float(entry['field2'])
            gas_presence = int(entry['field3'])
            overflow_status = int(entry['field4'])
            # Append features and labels to lists
            features.append([temperature, humidity, gas_presence])
            labels.append(overflow_status)
        return np.array(features), np.array(labels)
    else:
        print('Error retrieving data from ThingSpeak:', response.text)
        return None, None

# Retrieve data from ThingSpeak
CHANNEL_ID = '2453851'
READ_API_KEY = 'ZHHRUFIN5VC5PF5Y'
NUM_RESULTS = 1000  # Number of entries to retrieve

X, y = retrieve_data(CHANNEL_ID, READ_API_KEY, NUM_RESULTS)

# Print the retrieved data
print("Features (X):")
print(X)
print("Labels (y):")
print(y)

# Preprocess data
X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2, random_state=42)
scaler = StandardScaler()
X_train_scaled = scaler.fit_transform(X_train)
X_test_scaled = scaler.transform(X_test)

# Train Random Forest classifier
model = RandomForestClassifier(n_estimators=100, random_state=42)
model.fit(X_train_scaled, y_train)

# Evaluate model
y_pred = model.predict(X_test_scaled)
accuracy = accuracy_score(y_test, y_pred)
print("Accuracy:", accuracy)

# Function to predict overflow likelihood based on real-time data from ThingSpeak
def predict_overflow(channel_id, read_api_key):
    # Retrieve latest data from ThingSpeak
    latest_data = retrieve_data(channel_id, read_api_key, num_results=1)
    if latest_data:
        # Select the last entry (latest data point)
        latest_features = latest_data[0][-1]
        # Preprocess the latest data
        latest_features_scaled = scaler.transform([latest_features])
        # Predict overflow likelihood using the trained model
        overflow_likelihood = model.predict(latest_features_scaled)
        return overflow_likelihood
    else:
        print("Error: Failed to retrieve latest data from ThingSpeak")
        return None

# Function to predict overflow time
def predict_overflow_time(channel_id, read_api_key):
    # Predict overflow
    overflow_likelihood = predict_overflow(channel_id, read_api_key)
    if overflow_likelihood is not None:
        if overflow_likelihood == 1:
            # Estimate time until next overflow
            time_until_overflow = datetime.timedelta(minutes=10)  # Adjust as needed
            overflow_time = datetime.datetime.now() + time_until_overflow
            print("Estimated time until next overflow:", overflow_time.strftime("%Y-%m-%d %H:%M:%S"))
        else:
            print("No overflow predicted")
    else:
        print("Error: Failed to predict overflow")

# Test real-time overflow time prediction
predict_overflow_time(CHANNEL_ID, READ_API_KEY)
     