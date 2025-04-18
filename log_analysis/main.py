import pandas as pd
import matplotlib.pyplot as plt
from matplotlib import gridspec

# Load syscall log
df = pd.read_json("/tmp/syscalls_log.json")

# Categorize syscalls
category_map = {
    # File I/O
    'read': 'File I/O',
    'write': 'File I/O',
    'openat': 'File I/O',
    'close': 'File I/O',
    'access': 'File I/O',
    'newfstatat': 'File I/O',

    # Memory
    'brk': 'Memory',
    'mmap': 'Memory',
    'munmap': 'Memory',

    # Process
    'execve': 'Process',
    'getpid': 'Process',

    # Threading
    'futex': 'Threading',
    'rseq': 'Threading',

    # Timing
    'clock_nanosleep': 'Time',

    # Networking
    'socket': 'Networking',
    'connect': 'Networking',
    'accept': 'Networking',
    'sendto': 'Networking',
    'recvfrom': 'Networking',
    'sendmsg': 'Networking',
    'recvmsg': 'Networking',
    'shutdown': 'Networking',
    'bind': 'Networking',
    'listen': 'Networking',
    'getsockname': 'Networking',
    'getpeername': 'Networking',
    'socketpair': 'Networking',
    'setsockopt': 'Networking',
    'getsockopt': 'Networking',
}

df['category'] = df['name'].map(category_map).fillna("Other")

# Prepare figure with 2x2 layout
fig = plt.figure(figsize=(20, 10))
gs = gridspec.GridSpec(2, 2, height_ratios=[2, 1])


# Plot 1: Syscall frequency
# Full-width bar chart on top
ax1 = fig.add_subplot(gs[0, :])
df['name'].value_counts().plot(kind='bar', ax=ax1)
ax1.set_title("Syscall Frequency")
ax1.set_xlabel("Syscall")
ax1.set_ylabel("Count")
ax1.tick_params(axis='x', rotation=0)

# Pie chart (top-right of bottom row)
# ax2 = fig.add_subplot(gs[1, 0])
# df['success'].value_counts().plot(kind='pie', ax=ax2, autopct='%1.1f%%', labels=["Success", "Failure"])
# ax2.set_title("Success vs Failure")
# ax2.set_ylabel("")

# Category bar chart (bottom-left of bottom row)
ax3 = fig.add_subplot(gs[1, 1])
df['category'].value_counts().plot(kind='bar', ax=ax3, color='skyblue')
ax3.set_title("Syscall Categories")
ax3.set_xlabel("Category")
ax3.set_ylabel("Count")
ax3.tick_params(axis='x', rotation=0)

# optional, change buttom left to error type chart (comment the pie chart)
ax2 = fig.add_subplot(gs[1, 0])

# Filter only failed syscalls
failed_df = df[df['success'] == False]

failed_df['errorType'].value_counts().plot(kind='bar', ax=ax2, color='red')
ax2.set_title("Error Types")
ax2.set_xlabel("Category")
ax2.set_ylabel("Count")
ax2.tick_params(axis='x', rotation=0)

plt.tight_layout()
plt.show()
