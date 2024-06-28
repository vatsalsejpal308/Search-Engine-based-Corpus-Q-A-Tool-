import openai
import sys
import os  # Add this import

if __name__ == '__main__':
    
    # python3 <filename> API_KEY num_paragraphs query.txt
    if len(sys.argv) < 4:
        print("Usage: python api_call.py API_KEY num_paragraphs query.txt")
        sys.exit(1)

    # Read the API key from the command line
    openai.api_key = sys.argv[1]
    num_paragraphs = int(sys.argv[2])
    print(num_paragraphs)

    # Specify the directory where files are located
    base_directory = 'C:\IIT DELHI\SEM_3\COL106\Assignments\gradescope runner A7'

    # Read the paragraphs from the files
    paragraphs = []

    for i in range(num_paragraphs):
        filename = os.path.join(base_directory, 'paragraph_' + str(i) + '.txt')
        print(filename)
        with open(filename, 'r') as f:
            paragraphs.append(f.read())
            paragraphs.append('\n')
    
    # add query
    query_file = os.path.join(base_directory, sys.argv[3])
    with open(query_file, 'r') as f:
        query = f.read()
        paragraphs.append(query)
        paragraphs.append('\n')

    # convert paragraphs to a single string
    paragraphs = '\n'.join(paragraphs)

    print(paragraphs)

    response = openai.Completion.create(
        engine="text-davinci-003",  # Choose the appropriate engine
        prompt=paragraphs,
        temperature=0.7,
        max_tokens=150
    )

    reply = response.choices[0].text.strip()
    print(reply)

    # Write the response to a file
    with open(os.path.join(base_directory, "response.txt"), "w") as response_file:
        response_file.write(reply)
