import stats

# Example usage
numbers = [1.0, 2.0, 2.0, 3.0, 4.0, 5.0]

# Test all functions
print(f"Sum: {stats.sum(numbers)}")          # 17.0
print(f"Average: {stats.average(numbers)}")  # 2.833...
print(f"Std Dev: {stats.stddev(numbers)}")   # ~1.329...
print(f"Mode: {stats.mode(numbers)}")        # 2.0
print(f"Count: {stats.count(numbers)}")      # 6
