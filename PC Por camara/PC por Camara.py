# Importar las librerías necesarias
import cv2
import pyautogui

# Inicializar la cámara
camera = cv2.VideoCapture(0)
camera.set(cv2.CAP_PROP_FRAME_WIDTH, 640)
camera.set(cv2.CAP_PROP_FRAME_HEIGHT, 480)


# Verificar que la cámara se haya abierto correctamente
if not camera.isOpened():
    print("No se pudo abrir la cámara.")
    exit()

# Cargar el clasificador de cascada de Haar para la mano
hand_cascade = cv2.CascadeClassifier('haarcascade_hand.xml')

# Verificar que se haya cargado el clasificador correctamente
if hand_cascade.empty():
    print("No se pudo cargar el clasificador de cascada de Haar.")
    exit()

# Definir las variables para los límites del área del pecho
chest_x_min = 100
chest_x_max = 200
chest_y_min = 100
chest_y_max = 200

# Habilitar el modo FAILSAFE de PyAutoGUI
pyautogui.FAILSAFE = True

while True:
    # Capturar una imagen de la cámara
    _, frame = camera.read()
    frame = cv2.resize(frame, (0, 0), fx=0.5, fy=0.5)


    # Convertir la imagen a escala de grises
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)

    # Detectar movimientos de mano en la imagen
    hands = hand_cascade.detectMultiScale(gray)

    # Recorrer la lista de manos detectadas
    for (x, y, w, h) in hands:
        # Si la mano derecha está sobre el área del pecho
        # y el modo FAILSAFE está deshabilitado, habilitar el modo FAILSAFE
        if x > chest_x_min and x < chest_x_max and y > chest_y_min and y < chest_y_max and not pyautogui.FAILSAFE:
            pyautogui.FAILSAFE = True

        # Si la mano izquierda está sobre el área del pecho
        # y el modo FAILSAFE está habilitado, deshabilitar el modo FAILSAFE
        elif x > chest_x_min and x < chest_x_max and y > chest_y_min and y < chest_y_max and pyautogui.FAILSAFE:
            pyautogui.FAILSAFE = False

        # Mover el cursor del ratón a la posición de la mano
        # si el modo FAILSAFE está deshabilitado
        if not pyautogui.FAILSAFE:
            pyautogui.moveTo(x, y)

        # Si se han detectado dos manos
        if len(hands) == 2:
            # Obtener la posición de las dos manos
            x1, y1, w1, h1 = hands[0]
            x2, y2, w2, h2 = hands[1]

            # Si ambas manos están levantadas a la altura del pecho
            # y el modo FAILSAFE está deshabilitado, escribir en el teclado virtual
            if y1 > chest_y_min and y1 < chest_y_max and y2 > chest_y_min and y2 < chest_y_max and not pyautogui.FAILSAFE:
                # Escribir en el teclado virtual
                pyautogui.typewrite('Hola, estoy escribiendo con mis manos!')
            # Si ambas manos están levantadas a la altura del pecho
            # y están formando una cruz con los dedos índice, cerrar la funcionalidad
            elif y1 > chest_y_min and y1 < chest_y_max and y2 > chest_y_min and y2 < chest_y_max and x1 < x2 and y1 < y2:
                break
        # Mostrar la imagen en la ventana
        cv2.imshow('Webcam', frame)

        # Pausar el loop por un tiempo específico
        cv2.waitKey(30)

    # Salir del bucle si se pulsa la tecla 'q'
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

# Liberar la cámara y cerrar la ventana
camera.release()
cv2.destroyAllWindows()
