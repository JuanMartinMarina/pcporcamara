#include <opencv2/opencv.hpp>
#include <iostream>
#include <cmath>
#include <algorithm>

using namespace std;
using namespace cv;

int main(int argc, char* argv[])
{
    // Inicializar la cámara
    VideoCapture camera(0);
    if (!camera.isOpened()) {
        cerr << "ERROR: No se pudo abrir la cámara." << endl;
        return 1;
    }

    camera.set(CAP_PROP_FRAME_WIDTH, 640);
    camera.set(CAP_PROP_FRAME_HEIGHT, 480);

    // Cargar el clasificador de cascada de Haar para la mano
    CascadeClassifier hand_cascade;
    if (!hand_cascade.load("haarcascade_hand.xml")) {
        cerr << "ERROR: No se pudo cargar el clasificador de cascada de Haar." << endl;
        return 1;
    }

    // Definir las variables para los límites del área del pecho
    int chest_x_min = 100;
    int chest_x_max = 200;
    int chest_y_min = 100;
    int chest_y_max = 200;

    bool pyautogui_failsafe = true;

    while (true)
    {
        // Capturar una imagen de la cámara
        Mat frame;
        if (!camera.read(frame)) {
            cerr << "ERROR: No se pudo leer un frame de la cámara." << endl;
            break;
        }
        resize(frame, frame, Size(0, 0), 0.5, 0.5);

        // Convertir la imagen a escala de grises
        Mat gray;
        cvtColor(frame, gray, COLOR_BGR2GRAY);

        // Detectar movimientos de mano en la imagen
        vector<Rect> hands;
        hand_cascade.detectMultiScale(gray, hands);

        // Recorrer la lista de manos detectadas
        for (size_t i = 0; i < hands.size(); i++)
        {
            int x = hands[i].x;
            int y = hands[i].y;
            int w = hands[i].width;
            int h = hands[i].height;

            // Si la mano derecha está sobre el área del pecho
            // y el modo FAILSAFE está deshabilitado, habilitar el modo FAILSAFE
            if (x > chest_x_min && x < chest_x_max && y > chest_y_min && y < chest_y_max && !pyautogui_failsafe) {
                pyautogui_failsafe = true;
            }

            // Si la mano izquierda está sobre el área del pecho
            // y el modo FAILSAFE está habilitado, deshabilitar el modo FAILSAFE
            else if (x > chest_x_min && x < chest_x_max && y > chest_y_min && y < chest_y_max && pyautogui_failsafe) {
                pyautogui_failsafe = false;
            }

            // Mover el cursor del ratón a la posición de la mano
            // si el modo FAILSAFE está deshabilitado
            if (!pyautogui_failsafe) {
                SetCursorPos(x, y);
            }
        }

        // Si se han detectado dos manos
        if (hands.size() == 2)
        {
            // Obtener la posición de las dos manos
            int x1 = hands[0].x;
            int y1 = hands[0].y;
            int w1 = hands[0].width;
            int h1 = hands[0].height;
            int x2 = hands[1].x;
            int y2 = hands[1].y;
            int w2 = hands[1].width;
            int h2 = hands[1].height;

            // Si ambas manos están levantadas a la altura del pecho
            // y el modo FAILSAFE está deshabilitado, escribir en el teclado virtual
            if (y1 > chest_y_min && y1 < chest_y_max && y2 > chest_y_min && y2 < chest_y_max && !pyautogui_failsafe)
            {
                // Escribir en el teclado virtual
                keybd_event(VkKeyScan('H'), 0, 0, 0);
                keybd_event(VkKeyScan('H'), 0, KEYEVENTF_KEYUP, 0);
                keybd_event(VkKeyScan('E'), 0, 0, 0);
                keybd_event(VkKeyScan('E'), 0, KEYEVENTF_KEYUP, 0);
                keybd_event(VkKeyScan('L'), 0, 0, 0);
                keybd_event(VkKeyScan('L'), 0, KEYEVENTF_KEYUP, 0);
                keybd_event(VkKeyScan('L'), 0, 0, 0);
                keybd_event(VkKeyScan('L'), 0, KEYEVENTF_KEYUP, 0);
                keybd_event(VkKeyScan('O'), 0, 0, 0);
                keybd_event(VkKeyScan('O'), 0, KEYEVENTF_KEYUP, 0);
            }
        }

        // Mostrar la imagen de la cámara
        imshow("Camera", frame);

        // Si se ha pulsado la tecla 'q', salir del bucle
        if (waitKey(1) == 'q') {
            break;
        }
    }

    return 0;
}