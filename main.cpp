#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <fstream>
#include <vector>
#include "CharUtils.h"
#include "Admin.h"
#include "Patient.h"
#include "Doctor.h"
#include "FileHandler.h"
#include "Storage.h"
#include "Appointment.h"
#include "Bill.h"
#include "Prescription.h"

static const int   WIN_W = 900;
static const int   WIN_H = 650;
static const int   INPUT_MAX = 50;
static const int   MAX_MSG_LINES = 120;
static const int   MAX_MSG_LEN = 160;
static const sf::Color C_BG{ 248, 253, 250 };
static const sf::Color C_HEADER_BG{ 76, 145, 112 };
static const sf::Color C_BTN{ 96, 170, 128 };
static const sf::Color C_BTN_HOV{ 116, 190, 148 };
static const sf::Color C_BTN_GREEN{ 86, 165, 120 };
static const sf::Color C_BTN_RED{ 135, 170, 145 };
static const sf::Color C_BTN_CYAN{ 104, 180, 140 };
static const sf::Color C_BTN_YELL{ 142, 180, 105 };
static const sf::Color C_WHITE{ 255, 255, 255 };
static const sf::Color C_OFF_WHITE{ 250, 255, 252 };
static const sf::Color C_MUTED{ 95, 125, 105 };
static const sf::Color C_INPUT_BG{ 255, 255, 255 };
static const sf::Color C_INPUT_ACT{ 237, 248, 242 };
static const sf::Color C_BORDER{ 195, 220, 205 };
static const sf::Color C_ACCENT{ 150, 210, 175 };
static const sf::Color C_PANEL{ 255, 255, 255 };
static const sf::Color C_TITLE{ 46, 105, 75 };
static const sf::Color C_DARK_TEXT{ 40, 90, 65 };
static sf::Font gFont;
static void charAppend(char buf[], char c, int maxSize)
{
    int len = CharUtils::length(buf);
    if (len < maxSize - 1) {
        buf[len] = c;
        buf[len + 1] = '\0';
    }
}
static void charBackspace(char buf[])
{
    int len = CharUtils::length(buf);
    if (len > 0)
        buf[len - 1] = '\0';
}
static void itochar(int val, char out[], int maxSize)
{
    if (maxSize < 2) return;
    bool neg = (val < 0);
    if (neg)
        val = -val;
    char tmp[24];
    int ti = 0;
    if (val == 0)
        tmp[ti++] = '0';
    while (val > 0)
    {
        tmp[ti++] = '0' + val % 10;
        val /= 10;
    }

    int pos = 0;
    if (neg && pos < maxSize - 1)
        out[pos++] = '-';
    for (int k = ti - 1; k >= 0 && pos < maxSize - 1; k--)
    {
        out[pos++] = tmp[k];
    }
    out[pos] = '\0';
}

static int charToInt(const char buf[])
{
    int i = 0;
    bool neg = false;

    if (buf[0] == '-') 
    {
        neg = true;
        i = 1;
    }
    int r = 0;

    while (buf[i] >= '0' && buf[i] <= '9') {
        r = r * 10 + (buf[i] - '0');
        i++;
    }
    return neg ? -r : r;
}

static bool isValidInt(const char buf[])
{
    int i = 0;

    if (buf[0] == '-')
        i = 1;

    if (buf[i] == '\0')
        return false;

    while (buf[i]) {
        if (buf[i] < '0' || buf[i] > '9')
            return false;
        i++;
    }

    return true;
}

static void charConcat(char dest[], const char src[], int maxSize)
{
    int dlen = CharUtils::length(dest);
    int i = 0;

    while (src[i] && dlen + i < maxSize - 1) {
        dest[dlen + i] = src[i];
        i++;
    }

    dest[dlen + i] = '\0';
}

static void drawAt(sf::RenderWindow& w, const char* str,float x, float y, int size, sf::Color col)
{
    sf::Text t(str, gFont, size);
    t.setFillColor(col);
    t.setPosition(x, y);
    w.draw(t);
}

static void drawCentered(sf::RenderWindow& w, const char* str,int size, sf::Color col, float y)
{
    sf::Text t(str, gFont, size);
    t.setFillColor(col);
    sf::FloatRect b = t.getLocalBounds();
    t.setPosition((WIN_W - b.width) / 2.f, y);
    w.draw(t);
}

static void drawHeader(sf::RenderWindow& w, const char* subtitle)
{
    sf::RectangleShape bar({ (float)WIN_W, 70.f });
    bar.setFillColor(C_HEADER_BG);
    w.draw(bar);
    drawCentered(w, "MediCore Hospital Management System", 22, C_WHITE, 14.f);
    sf::RectangleShape line({ (float)WIN_W, 3.f });
    line.setPosition(0.f, 70.f);
    line.setFillColor(C_ACCENT);
    w.draw(line);
    drawCentered(w, subtitle, 32, C_TITLE, 105.f);
}

struct InputBox {
    sf::RectangleShape box;
    char   text[INPUT_MAX];
    char   label[60];
    bool   active;
    bool   password;
    void init(const char* lbl, float x, float y,
        float w = 420.f, bool pwd = false)
    {
        CharUtils::copy(label, lbl, 60);
        text[0] = '\0';
        active = false;
        password = pwd;
        box.setSize({ w, 44.f });
        box.setPosition(x, y);
        box.setFillColor(C_INPUT_BG);
        box.setOutlineThickness(2.f);
        box.setOutlineColor(C_BORDER);
    }
    void handleEvent(const sf::Event& e)
    {
        if (e.type == sf::Event::MouseButtonPressed)
        {
            active = box.getGlobalBounds().contains((float)e.mouseButton.x,(float)e.mouseButton.y);
            box.setOutlineColor(active ? C_ACCENT : C_BORDER);
            box.setFillColor(active ? C_INPUT_ACT : C_INPUT_BG);
        }
        if (active && e.type == sf::Event::TextEntered)
        {
            if (e.text.unicode == 8)
                charBackspace(text);
            else if (e.text.unicode >= 32 && e.text.unicode < 127)
                charAppend(text, (char)e.text.unicode, INPUT_MAX);
        }
    }

    void draw(sf::RenderWindow& w) const
    {
        w.draw(box);
        drawAt(w, label,
            box.getPosition().x,
            box.getPosition().y - 24.f,
            16,
            C_MUTED
        );
        char shown[INPUT_MAX];
        if (password)
        {
            int len = CharUtils::length(text);
            for (int i = 0; i < len; i++)
                shown[i] = '*';
            shown[len] = '\0';
        }
        else 
        {
            CharUtils::copy(shown, text, INPUT_MAX);
        }
        bool empty = (shown[0] == '\0');

        drawAt(w, empty ? "Click to type..." : shown,box.getPosition().x + 12.f, box.getPosition().y + 10.f,18, empty ? C_MUTED : C_DARK_TEXT );

        if (active) { sf::Text tmp(shown, gFont, 18);
            float cx = box.getPosition().x + 12.f + tmp.getLocalBounds().width + 2.f;
            sf::RectangleShape cur({ 2.f, 22.f });
            cur.setPosition(cx, box.getPosition().y + 10.f);
            cur.setFillColor(C_HEADER_BG);
            w.draw(cur);
        }
    }
    void clear()
    {
        text[0] = '\0';
    }
};
struct Button {
    sf::RectangleShape box;
    char label[80];
    sf::Color col;
    void init(const char* lbl, float x, float y,float w = 420.f, float h = 48.f, sf::Color c = C_BTN)
    {
        CharUtils::copy(label, lbl, 80);
        col = c;
        box.setSize({ w, h });
        box.setPosition(x, y);
        box.setFillColor(c);
        box.setOutlineThickness(0.f);
    }

    bool clicked(const sf::Event& e) const
    {
        return e.type == sf::Event::MouseButtonPressed &&
            box.getGlobalBounds().contains((float)e.mouseButton.x,(float)e.mouseButton.y);
    }

    void draw(sf::RenderWindow& w) const
    {
        sf::RectangleShape b = box;

        sf::Vector2i mp = sf::Mouse::getPosition(w);
        bool hover = b.getGlobalBounds().contains((float)mp.x, (float)mp.y);

        if (hover)
            b.setFillColor(C_BTN_HOV);
        else
            b.setFillColor(col);

        w.draw(b);

        sf::Text t(label, gFont, 18);
        t.setFillColor(C_WHITE);

        sf::FloatRect tb = t.getLocalBounds();

        t.setPosition(
            box.getPosition().x + (box.getSize().x - tb.width) / 2.f,
            box.getPosition().y + (box.getSize().y - tb.height) / 2.f - 5.f
        );

        w.draw(t);
    }
};


struct MsgPanel {
    sf::RectangleShape box;
    char      lines[MAX_MSG_LINES][MAX_MSG_LEN];
    sf::Color colors[MAX_MSG_LINES];
    int       count, scroll, visible;

    void init(float x, float y, float w, float h)
    {
        count = 0;
        scroll = 0;
        visible = (int)(h / 22);

        box.setPosition(x, y);
        box.setSize({ w, h });
        box.setFillColor(C_PANEL);
        box.setOutlineThickness(1.5f);
        box.setOutlineColor(C_BORDER);
    }

    void add(const char* s, sf::Color c = C_DARK_TEXT)
    {
        if (count >= MAX_MSG_LINES)
            return;

        CharUtils::copy(lines[count], s, MAX_MSG_LEN);
        colors[count++] = c;

        if (count > visible)
            scroll = count - visible;
    }

    void clear()
    {
        count = 0;
        scroll = 0;
    }

    void handleKey(const sf::Event& e)
    {
        if (e.type != sf::Event::KeyPressed)
            return;

        if (e.key.code == sf::Keyboard::Up && scroll > 0)
            --scroll;

        if (e.key.code == sf::Keyboard::Down && scroll < count - visible)
            ++scroll;
    }

    void draw(sf::RenderWindow& w) const
    {
        w.draw(box);

        float px = box.getPosition().x + 10.f;
        float py = box.getPosition().y + 6.f;

        int end = scroll + visible;
        if (end > count)
            end = count;

        for (int i = scroll; i < end; i++) {
            drawAt(w, lines[i], px, py + (i - scroll) * 22.f, 15, colors[i]);
        }

        if (count > visible) {
            char hint[48];
            hint[0] = '\0';

            charConcat(hint, "Scroll: Up/Down  (", 48);

            char n[8];
            itochar(scroll + 1, n, 8);
            charConcat(hint, n, 48);

            charConcat(hint, "/", 48);

            itochar(count, n, 8);
            charConcat(hint, n, 48);

            charConcat(hint, ")", 48);

            drawAt(w,
                hint,
                box.getPosition().x + 6.f,
                box.getPosition().y + box.getSize().y - 18.f,
                12,
                C_MUTED
            );
        }
    }
};

static void logSecurityEvent(const char* role, int id)
{
    std::ofstream log("security_log.txt", std::ios::app);

    if (!log.is_open())
        return;

    char idBuf[12];
    itochar(id, idBuf, 12);

    log << "FAILED LOGIN - Role: " << role
        << " | ID: " << idBuf
        << " | Locked after 3 attempts.\n";
}

template<typename T>
bool loginScreen(sf::RenderWindow& w, Storage<T>& storage,
    const char* role, int& outIndex)
{
    float cx = WIN_W / 2.f;

    sf::RectangleShape shadow({ 500.f, 370.f });
    shadow.setPosition(cx - 245.f, 156.f);
    shadow.setFillColor(sf::Color(180, 200, 185, 80));

    sf::RectangleShape card({ 500.f, 370.f });
    card.setPosition(cx - 250.f, 150.f);
    card.setFillColor(C_PANEL);
    card.setOutlineThickness(2.f);
    card.setOutlineColor(C_BORDER);

    InputBox idBox, pwBox;
    Button btnLogin, btnCancel;

    idBox.init("ID", cx - 210.f, 225.f, 420.f);
    pwBox.init("Password", cx - 210.f, 320.f, 420.f, true);

    btnLogin.init("Login", cx - 210.f, 415.f, 200.f, 48.f, C_BTN_GREEN);
    btnCancel.init("Cancel", cx + 10.f, 415.f, 200.f, 48.f, C_BTN_RED);

    char title[60];
    title[0] = '\0';

    charConcat(title, role, 60);
    charConcat(title, " Login", 60);

    char status[100];
    status[0] = '\0';

    int attempts = 0;
    bool locked = false;

    while (w.isOpen())
    {
        w.clear(C_BG);
        drawHeader(w, title);

        w.draw(shadow);
        w.draw(card);

        idBox.draw(w);
        pwBox.draw(w);

        btnLogin.draw(w);
        btnCancel.draw(w);

        if (status[0] != '\0')
            drawCentered(w, status, 16, C_HEADER_BG, 475.f);

        if (locked)
            drawCentered(w, "Session locked - contact admin.", 16, C_HEADER_BG, 500.f);

        char att[32];
        att[0] = '\0';

        charConcat(att, "Attempt ", 32);

        char n[4];
        itochar(attempts, n, 4);

        charConcat(att, n, 32);
        charConcat(att, " of 3", 32);

        drawCentered(w, att, 15, C_MUTED, 540.f);
        w.display();
        sf::Event ev;
        while (w.pollEvent(ev))
        {
            if (ev.type == sf::Event::Closed)
            {
                w.close();
                return false;
            }
            idBox.handleEvent(ev);
            pwBox.handleEvent(ev);

            bool doLogin = btnLogin.clicked(ev) ||
                (ev.type == sf::Event::KeyPressed &&
                    ev.key.code == sf::Keyboard::Enter);

            if (doLogin && !locked)
            {
                if (!isValidInt(idBox.text)) {
                    CharUtils::copy(status, "ID must be a number.", 100);
                    continue;
                }

                int id = charToInt(idBox.text);

                for (int i = 0; i < storage.size(); i++)
                {
                    T& rec = storage.getAll()[i];

                    if (rec.getId() == id) {
                        if (CharUtils::equals(rec.getPassword(), pwBox.text))
                        {
                            outIndex = i;
                            return true;
                        }

                        break;
                    }
                }

                ++attempts;

                if (attempts >= 3)
                {
                    locked = true;
                    logSecurityEvent(role, id);
                    CharUtils::copy(status, "Account locked.", 100);
                }
                else {
                    char rem[4];
                    itochar(3 - attempts, rem, 4);
                    CharUtils::copy(status, "Wrong ID or password. ", 100);
                    charConcat(status, rem, 100);
                    charConcat(status, " attempt(s) left.", 100);

                    pwBox.clear();
                }
            }

            if (btnCancel.clicked(ev))
                return false;

            if (ev.type == sf::Event::KeyPressed &&
                ev.key.code == sf::Keyboard::Escape)
                return false;
        }
    }

    return false;
}


static bool intDialog(sf::RenderWindow& w, const char* prompt, int& out)
{
    float cx = WIN_W / 2.f;
    sf::RectangleShape shadow({ 460.f, 240.f });
    shadow.setPosition(cx - 225.f, 206.f);
    shadow.setFillColor(sf::Color(180, 200, 185, 80));
    sf::RectangleShape card({ 460.f, 240.f });
    card.setPosition(cx - 230.f, 200.f);
    card.setFillColor(C_PANEL);
    card.setOutlineThickness(2.f);
    card.setOutlineColor(C_BORDER);
    InputBox box;
    Button btnOk, btnCan;
    box.init(prompt, cx - 210.f, 270.f, 420.f);
    btnOk.init("OK", cx - 210.f, 355.f, 200.f, 48.f, C_BTN_GREEN);
    btnCan.init("Cancel", cx + 10.f, 355.f, 200.f, 48.f, C_BTN_RED);
    char err[60];
    err[0] = '\0';

    while (w.isOpen())
    {
        w.clear(C_BG);
        drawHeader(w, prompt);
        w.draw(shadow);
        w.draw(card);
        box.draw(w);
        btnOk.draw(w);
        btnCan.draw(w);

        if (err[0])
            drawCentered(w, err, 16, C_HEADER_BG, 415.f);
        w.display();
        sf::Event e;
        while (w.pollEvent(e))
        {
            if (e.type == sf::Event::Closed) {
                w.close();
                return false;
            }
            box.handleEvent(e);
            bool ok = btnOk.clicked(e) ||
                (e.type == sf::Event::KeyPressed &&
                    e.key.code == sf::Keyboard::Enter);
            if (ok) {
                if (isValidInt(box.text)) {
                    out = charToInt(box.text);
                    return true;
                }
                CharUtils::copy(err, "Please enter a valid number.", 60);
            }
            if (btnCan.clicked(e))
                return false;

            if (e.type == sf::Event::KeyPressed &&
                e.key.code == sf::Keyboard::Escape)
                return false;
        }
    }

    return false;
}

struct MenuItem {
    char label[80];
    sf::Color col;
};

static int menuScreen(sf::RenderWindow& w,
    const char* title,
    const std::vector<MenuItem>& items)
{
    bool twoCols = items.size() > 8;

    float btnW = twoCols ? 360.f : 400.f;
    float btnH = twoCols ? 43.f : 42.f;
    float gapX = 28.f;
    float gapY = twoCols ? 12.f : 8.f;

    int rows = twoCols ? ((int)items.size() + 1) / 2 : (int)items.size();

    float totalW = twoCols ? (btnW * 2.f + gapX) : btnW;
    float totalH = rows * btnH + (rows - 1) * gapY;

    float cardW = totalW + 70.f;
    float cardH = totalH + 55.f;

    float cardX = (WIN_W - cardW) / 2.f;
    float cardY = twoCols ? 155.f : 150.f;

    float startX = cardX + 35.f;
    float startY = cardY + 28.f;

    std::vector<Button> buttons(items.size());

    for (int i = 0; i < (int)items.size(); i++)
    {
        int col = twoCols ? i % 2 : 0;
        int row = twoCols ? i / 2 : i;

        float x = startX + col * (btnW + gapX);
        float y = startY + row * (btnH + gapY);

        buttons[i].init(items[i].label, x, y, btnW, btnH, items[i].col);
    }

    while (w.isOpen())
    {
        w.clear(C_BG);
        drawHeader(w, title);

        sf::RectangleShape shadow({ cardW, cardH });
        shadow.setPosition(cardX + 5.f, cardY + 6.f);
        shadow.setFillColor(sf::Color(180, 200, 185, 90));
        w.draw(shadow);

        sf::RectangleShape card({ cardW, cardH });
        card.setPosition(cardX, cardY);
        card.setFillColor(C_PANEL);
        card.setOutlineThickness(2.f);
        card.setOutlineColor(C_BORDER);
        w.draw(card);

        for (auto& b : buttons)
            b.draw(w);

        drawCentered(w,
            "Use mouse to select an option. Press Esc to go back.",
            14,
            C_MUTED,
            WIN_H - 30.f
        );

        w.display();

        sf::Event e;

        while (w.pollEvent(e))
        {
            if (e.type == sf::Event::Closed)
            {
                w.close();
                return -1;
            }

            if (e.type == sf::Event::KeyPressed &&
                e.key.code == sf::Keyboard::Escape)
                return -1;

            for (int i = 0; i < (int)buttons.size(); i++)
            {
                if (buttons[i].clicked(e))
                    return i;
            }
        }
    }

    return -1;
}
void mainMenu(sf::RenderWindow&, Storage<Admin>&, Storage<Doctor>&, Storage<Patient>&, Storage<Appointment>&,Storage<Bill>&, Storage<Prescription>&);

void patientMenu(sf::RenderWindow&, Patient&, Storage<Doctor>&,Storage<Patient>&, Storage<Appointment>&,Storage<Bill>&, Storage<Prescription>&);

void doctorMenu(sf::RenderWindow&, Doctor&, Storage<Patient>&, Storage<Appointment>&, Storage<Bill>&, Storage<Prescription>&);

void adminMenu(sf::RenderWindow&, Admin&, Storage<Doctor>&, Storage<Patient>&, Storage<Appointment>&, Storage<Bill>&);

void mainMenu(sf::RenderWindow& w, Storage<Admin>& admins, Storage<Doctor>& doctors, Storage<Patient>& patients, Storage<Appointment>& appointments, Storage<Bill>& bills, Storage<Prescription>& prescriptions)
{
    std::vector<MenuItem> items = {
        { "Patient Login", C_BTN },
        { "Doctor Login",  C_BTN },
        { "Admin Login",   C_BTN },
        { "Exit",          C_BTN_RED }
    };

    while (w.isOpen())
    {
        int choice = menuScreen(w, "Welcome to MediCore", items);

        if (choice == 0)
        {
            int idx = -1;

            if (loginScreen(w, patients, "Patient", idx))
                patientMenu(w, patients.getAll()[idx],
                    doctors, patients, appointments, bills, prescriptions);
        }
        else if (choice == 1)
        {
            int idx = -1;

            if (loginScreen(w, doctors, "Doctor", idx))
                doctorMenu(w, doctors.getAll()[idx],
                    patients, appointments, bills, prescriptions);
        }
        else if (choice == 2)
        {
            int idx = -1;

            if (loginScreen(w, admins, "Admin", idx))
                adminMenu(w, admins.getAll()[idx],
                    doctors, patients, appointments, bills);
        }
        else 
        {
            w.close();
            return;
        }
    }
}

void patientMenu(sf::RenderWindow& w, Patient& patient,Storage<Doctor>& doctors, Storage<Patient>& patients,Storage<Appointment>& appointments, Storage<Bill>& bills, Storage<Prescription>& prescriptions)
{
    char hdr[80];
    hdr[0] = '\0';
    charConcat(hdr, "Patient: ", 80);
    charConcat(hdr, patient.getName(), 80);
    std::vector<MenuItem> items = {
        { "Book Appointment",      C_BTN },
        { "Cancel Appointment",    C_BTN },
        { "View My Appointments",  C_BTN },
        { "View Medical Records",  C_BTN },
        { "View My Bills",         C_BTN },
        { "Pay Bill",              C_BTN },
        { "Top Up Balance",        C_BTN },
        { "Logout",                C_BTN_RED }
    };

    while (w.isOpen())
    {
        int choice = menuScreen(w, hdr, items);

        if (choice == 0)
            patient.bookAppointment(doctors, appointments, bills);

        else if (choice == 1)
            patient.cancelAppointment(appointments, bills, doctors, patients);

        else if (choice == 2)
            patient.viewAppointments(appointments, doctors);

        else if (choice == 3)
            patient.viewMedicalRecords(prescriptions, appointments, doctors);

        else if (choice == 4)
            patient.viewBills(bills);

        else if (choice == 5)
            patient.payBill(bills, patients);

        else if (choice == 6)
            patient.topUpBalance(patients);

        else
            return;
    }
}

void doctorMenu(sf::RenderWindow& w, Doctor& doctor, Storage<Patient>& patients,Storage<Appointment>& appointments, Storage<Bill>& bills, Storage<Prescription>& prescriptions)
{
    char hdr[80];
    hdr[0] = '\0';
    charConcat(hdr, "Dr. ", 80);
    charConcat(hdr, doctor.getName(), 80);

    std::vector<MenuItem> items = {
        { "View Today's Appointments",  C_BTN },
        { "Mark Appointment Complete",  C_BTN_GREEN },
        { "Mark Appointment No-Show",   C_BTN_RED },
        { "Write Prescription",         C_BTN },
        { "View Patient History",       C_BTN },
        { "Logout",                     C_BTN_RED }
    };

    while (w.isOpen())
    {
        int choice = menuScreen(w, hdr, items);

        if (choice == 0)
            doctor.viewTodaysAppointments(appointments, patients);

        else if (choice == 1)
            doctor.markAppointmentComplete(appointments);

        else if (choice == 2)
            doctor.markAppointmentNoShow(appointments, bills);

        else if (choice == 3)
            doctor.writePrescription(prescriptions, appointments, patients);

        else if (choice == 4)
        {
            int pid = 0;

            if (intDialog(w, "Enter Patient ID", pid))
                doctor.viewPatientMedicalHistory(pid, appointments, prescriptions, patients);
        }

        else
            return;
    }
}

void adminMenu(sf::RenderWindow& w, Admin& admin, Storage<Doctor>& doctors, Storage<Patient>& patients, Storage<Appointment>& appointments, Storage<Bill>& bills)
{
    std::vector<MenuItem> items = {
        { "Add Doctor",             C_BTN },
        { "Remove Doctor",          C_BTN_RED },
        { "Add Patient",            C_BTN },
        { "Remove Patient",         C_BTN_RED },
        { "View All Patients",      C_BTN },
        { "View All Doctors",       C_BTN },
        { "View All Appointments",  C_BTN },
        { "View Unpaid Bills",      C_BTN_YELL },
        { "Discharge Patient",      C_BTN_CYAN },
        { "View Security Log",      C_BTN_YELL },
        { "Generate Daily Report",  C_BTN_CYAN },
        { "Logout",                 C_BTN_RED }
    };

    while (w.isOpen())
    {
        int choice = menuScreen(w, "Admin Panel", items);

        if (choice == 0)
            admin.addDoctor(doctors);

        else if (choice == 1)
            admin.removeDoctor(doctors, appointments);

        else if (choice == 2)
            admin.addPatient(patients);

        else if (choice == 3)
            admin.removePatient(patients, appointments, bills);

        else if (choice == 4)
            admin.viewAllPatients(patients);

        else if (choice == 5)
            admin.viewAllDoctors(doctors);

        else if (choice == 6)
        {
            FileHandler::loadAppointments("appointment.txt", appointments);
            admin.viewAllAppointments(appointments);
        }

        else if (choice == 7)
        {
            FileHandler::loadBills("bill.txt", bills);
            admin.viewUnpaidBills(bills);
        }

        else if (choice == 8)
            admin.dischargePatient(patients, appointments, bills);

        else if (choice == 9)
            admin.viewSecurityLog();

        else if (choice == 10)
            admin.generateDailyReport(appointments, bills);

        else
            return;
    }
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(WIN_W, WIN_H), "MediCore Hospital Management System", sf::Style::Titlebar | sf::Style::Close);

    window.setFramerateLimit(60);

    if (!gFont.loadFromFile("assets/fonts/arial.ttf"))
        if (!gFont.loadFromFile("assets/fonts/game_over.ttf"))
            if (!gFont.loadFromFile("C:/Windows/Fonts/arial.ttf"))
                return 1;

    Storage<Doctor>       doctors;
    Storage<Patient>      patients;
    Storage<Appointment>  appointments;
    Storage<Bill>         bills;
    Storage<Prescription> prescriptions;
    Storage<Admin>        admins;

    FileHandler::loadadmins("admins.txt", admins);
    FileHandler::loadDoctors("doctors.txt", doctors);
    FileHandler::loadPatients("patients.txt", patients);
    FileHandler::loadAppointments("appointment.txt", appointments);
    FileHandler::loadBills("bill.txt", bills);
    FileHandler::loadPrescriptions("prescriptions.txt", prescriptions);

    mainMenu(window, admins, doctors, patients, appointments, bills, prescriptions);

    return 0;
}