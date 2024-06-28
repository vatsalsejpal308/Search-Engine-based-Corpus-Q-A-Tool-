import requests

# Set the API key
api_key = "vatsal-sejpal@llm-project-a7.iam.gserviceaccount.com"

# Read the query from file
with open("query.txt", "r") as f:
    query = f.read()

# Make a request to the PaLM API
headers = {
    "Authorization": f"Bearer {api_key}"
}
request_body = {
    "input": {
        "text": query
    }
}
response = requests.post("https://generativelanguage.googleapis.com/v1beta2/models/text-bison-001:generateText", headers=headers, json=request_body)

# Check the response status code
if response.status_code == 200:
    # Success!
    response_json = response.json()

    # Write the response to the console
    print(response_json["output"]["text"])

    # Write the response to file
    with open("response.txt", "w") as f:
        f.write(response_json["output"]["text"])
else:
    # An error occurred
    print(response.status_code)
    print(response.content)