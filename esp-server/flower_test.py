import cv2
import numpy as np
import matplotlib.pyplot as plt

def find_flower_center(img_path, output_path='flower_center.png'):
    # 1. Load image
    img = cv2.imread(img_path)
    if img is None:
        raise FileNotFoundError(f"Could not load image at {img_path}")

    # 2. Convert to HSV and mask out green background
    hsv = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)
    # these ranges may need tweaking for your exact foliage vs. flower colors
    lower_green = np.array([25, 40, 40])
    upper_green = np.array([90, 255, 255])
    mask_green = cv2.inRange(hsv, lower_green, upper_green)
    mask_flower = cv2.bitwise_not(mask_green)

    # 3. Clean up mask
    kernel = cv2.getStructuringElement(cv2.MORPH_ELLIPSE, (7,7))
    mask = cv2.morphologyEx(mask_flower, cv2.MORPH_CLOSE, kernel, iterations=2)
    mask = cv2.morphologyEx(mask, cv2.MORPH_OPEN, kernel, iterations=2)

    # 4. Find contours and pick the largest one
    contours, _ = cv2.findContours(mask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
    if not contours:
        raise RuntimeError("No flower-shaped contour found—try adjusting your HSV mask.")
    c = max(contours, key=cv2.contourArea)

    # 5. Compute centroid via image moments
    M = cv2.moments(c)
    if M['m00'] == 0:
        raise RuntimeError("Contour area is zero; can't compute centroid.")
    cx = int(M['m10'] / M['m00'])
    cy = int(M['m01'] / M['m00'])
    print(f"🌸 Flower center at: ({cx}, {cy})")

    # 6. Draw and save result
    out = img.copy()
    cv2.circle(out, (cx, cy), 5, (0, 0, 255), -1)         # red dot at center
    cv2.drawContours(out, [c], -1, (0, 255, 0), 2)        # green outline of flower

    # 7. Show with matplotlib (no Qt needed)
    rgb = cv2.cvtColor(out, cv2.COLOR_BGR2RGB)
    plt.figure(figsize=(6,6))
    plt.imshow(rgb)
    plt.axis('off')
    plt.title('Detected Flower Center')
    plt.show()

    # 8. Save the annotated image
    cv2.imwrite(output_path, out)
    print(f"✅ Result saved to {output_path}")

if __name__ == "__main__":
    import sys
    if len(sys.argv) != 2:
        print("Usage: python find_center.py path/to/flower.jpg")
        sys.exit(1)
    find_flower_center(sys.argv[1])