import torch
from PIL import Image
import matplotlib.pyplot as plt
import matplotlib.patches as patches

# Load the custom model
model = torch.hub.load('ultralytics/yolov5', 'custom', path='flower.pt', force_reload=True)

# Load and preprocess the image
img_path = 'flower.jpg'  # Replace with your image path
img = Image.open(img_path)

# Perform inference
results = model(img)

# Parse results
results.print()  # Print results to console

# Display results
results.show()  # Display image with bounding boxes

# Save results
results.save()  # Save image with bounding boxes to 'runs/detect/exp'
