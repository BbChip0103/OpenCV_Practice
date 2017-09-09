import cv2

cap = cv2.VideoCapture(file_path)

# Get frame size
# 3 : width, 4 : height
video_width = (int)(cap.get(3))
video_height = (int)(cap.get(4))

fourcc = cv2.VideoWriter_fourcc(*'DIVX')
out = cv2.VideoWriter('output.avi', fourcc, 30.0, (video_width, video_height))

while(cap.isOpened()):
    ret, frame = cap.read()

    # Convert gray color in image
    half_width = (int)(video_width / 2)

    for row in range(0, video_height) :
        for column in range(0, half_width) :
            color_tmp = 0
            # 0 : Blue, 1 : Green, 2 : Red
            for i_rgb in range(0, 3) :
                color_tmp += frame.item(row, column, i_rgb)

            # gray color = (red + green + blue) / 3
            color_tmp /= 3
            for i_rgb in range(0, 3) :
                frame.itemset((row, column, i_rgb), color_tmp)

            # Make boundry line
            frame.itemset((row, half_width, 0), 0)
            frame.itemset((row, half_width, 1), 0)
            frame.itemset((row, half_width, 2), 255)

    cv2.imshow('frame',frame)

    out.write(frame)

    if cv2.waitKey(1) & 0xFF == ord('q'):
        out.release()
        break

cap.release()
cv2.destroyAllWindows()

# If it is not homework, maybe i use these functions
# Additionaly, these are faster than my code

# Convert color function
# ex) out_frame = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)

# Region of Image(ROI)
# img[start of law : end of law, start of column : end of column]
# ex) frame = img[409:454, 817:884]

# Draw a line
# cv2.line(img, start, end, color, thickness)
# ex) img = cv2.line(img, (0, 0), (511, 511), (255, 0, 0), 5)
