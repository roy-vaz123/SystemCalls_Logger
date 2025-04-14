import pandas as pd
import matplotlib.pyplot as plt

# Load syscall log
df = pd.read_json("/tmp/syscalls_log.json")

# Categorize syscalls
category_map = {
    'read': 'File I/O', 'write': 'File I/O', 'openat': 'File I/O', 'close': 'File I/O', 'access': 'File I/O',
    'brk': 'Memory', 'mmap': 'Memory', 'munmap': 'Memory',
    'execve': 'Process', 'getpid': 'Process',
    'futex': 'Threading', 'rseq': 'Threading',
    'clock_nanosleep': 'Time', 'newfstatat': 'File I/O'
}
df['category'] = df['name'].map(category_map).fillna("Other")

# Prepare figure with 2x2 layout
fig, axes = plt.subplots(2, 2, figsize=(20, 10))

# Plot 1: Syscall frequency
df['name'].value_counts().plot(kind='bar', ax=axes[0, 0])
axes[0, 0].set_title("Syscall Frequency")
axes[0, 0].set_xlabel("Syscall")
axes[0, 0].set_ylabel("Count")
axes[0, 0].tick_params(axis='x', rotation=0)

# Plot 2: Success vs Failure (pie chart)
df['success'].value_counts().plot(
    kind='pie',
    ax=axes[0, 1],
    autopct='%1.1f%%',
    labels=['Success', 'Failure'] if len(df['success'].unique()) == 2 else df['success'].unique()
)
axes[0, 1].set_title("Success vs Failure")
axes[0, 1].set_ylabel("")

# Plot 3: Syscall categories
df['category'].value_counts().plot(kind='bar', ax=axes[1, 0], color='skyblue')
axes[1, 0].set_title("Syscall Categories")
axes[1, 0].set_xlabel("Category")
axes[1, 0].set_ylabel("Count")
axes[1, 0].tick_params(axis='x', rotation=0)

# Plot 4: Most common error types (if any)
errors = df[df['success'] == False]
if not errors.empty:
    errors['errorType'].value_counts().plot(kind='bar', ax=axes[1, 1], color='red')
    axes[1, 1].set_title("Common Error Types")
    axes[1, 1].set_xlabel("Error Type")
    axes[1, 1].set_ylabel("Count")
    axes[1, 1].tick_params(axis='x', rotation=0)
else:
    axes[1, 1].axis('off')
    axes[1, 1].text(0.5, 0.5, "No errors recorded", ha='center', va='center', fontsize=12)

plt.tight_layout()
plt.show()
