import google.generativeai as genai

# Set your API key
genai.configure(api_key="AIzaSyDoG1s9n2V8etcrULDOs8t0YpX7RWdDu6I")

data = {}

try:
    with open("ai_input.txt", "r") as f:
        for line in f:
            if ":" in line:
                key, value = line.strip().split(": ")
                data[key.lower()] = float(value)
except FileNotFoundError:
    print("❌ ai_input.txt not found. Run the C program first.")
    exit()

prompt = f"""
Here is my monthly financial data:

- Income: ${data.get('income', 0)}
- Education: ${data.get('education', 0)}
- Insurance: ${data.get('insurance', 0)}
- Groceries: ${data.get('groceries', 0)}
- Rent: ${data.get('rent', 0)}
- Entertainment: ${data.get('entertainment', 0)}
- Others: ${data.get('others', 0)}
- Total Expenditure: ${data.get('total expenditure', 0)}

Based on this, analyze my spending and suggest me in three short sentences line-by-line: 1) only if expenditure exceeds budget, analyze how to and by what percentage should I cut off expenses from 'low priority (3-6)' category and keep things under budget 2)else, only if budget exceeds expenditure, tell me how to improve my quality of life by increasing my spending by what percentage on 'high priority (1-3)' category and keep aside what percentage on savings for future. Make one of the two choices I gave you based on budget amount and expenses and discard the other. At the end, give one line of sarcastic suggestion just for fun.
"""

# Call Gemini
model = genai.GenerativeModel("models/gemini-1.5-flash")
response = model.generate_content(prompt)

print("\n--- Orion Financial Advice ---\n")
print(response.text)
