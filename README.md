# Cab Booking App (C++)

This is a console-based Cab Booking application I built as part of Project Phase 1 for my internship at Sqrock IT Solutions. The idea was to simulate how a real cab service like Ola or Uber works under the hood — user accounts, booking a ride, fare calculation, and keeping track of trip history — all using core C++ concepts.

## What it does

- Users can register with their name and phone number, then log in/log out.
- Once logged in, you can book a cab by entering pickup and drop locations, choosing a cab type (Mini, Sedan, or SUV), and entering the estimated distance.
- The app calculates an estimated fare based on the cab type and distance, and automatically adds a night charge if you're booking between 10 PM and 6 AM.
- You can view your current active booking, cancel it, or mark it as completed.
- Every trip gets saved, so you can pull up your full trip history at any time.

## How it's built

I used core C++ with OOP (separate `User` and `Trip` classes), STL containers like `vector`, and file handling to make the data persist between runs — so if you close the program and open it again, your account and trip history are still there. User records go into `users.txt` and trip data into `trips.txt`, both stored as simple pipe-separated (`|`) text so they're easy to read and debug.

## Technologies used

- Core C++
- Object-Oriented Programming (classes, encapsulation)
- STL (`vector`, `string`, `sstream`)
- File Handling (`ifstream` / `ofstream`)
- `<ctime>` for timestamps and night-charge detection

## How to run it

```bash
g++ -std=c++17 -o cab_booking cab_booking.cpp
./cab_booking
```

On Windows (VS Code terminal):

```powershell
g++ -std=c++17 -o cab_booking cab_booking.cpp
.\cab_booking.exe
```

## Things I'd add if I had more time

- A driver-side module where drivers can accept/reject rides and update trip status live
- GPS/location simulation instead of manually typing distance
- A basic rating system after a trip is completed
- Switching from flat text files to a small SQLite database

## What I learned

This project really helped me understand how to structure a menu-driven application cleanly using classes instead of dumping everything into `main()`. Handling login sessions and making sure data survives across program restarts (via file I/O) was probably the trickiest part — and a good reminder of why real apps need proper databases instead of text files once things get more complex.

---
Built as part of Project Phase 1, Sqrock IT Solutions Internship.


<img width="388" height="640" alt="1" src="https://github.com/user-attachments/assets/6e9c4eeb-2cad-4808-bcfd-2da1646bbb11" />
<img width="511" height="759" alt="2" src="https://github.com/user-attachments/assets/344e7c53-85e8-49bf-b5a9-b59df6485709" />
<img width="450" height="616" alt="3" src="https://github.com/user-attachments/assets/4b7ecbad-60f8-44be-937e-8118865a1e12" />
<img width="508" height="615" alt="4" src="https://github.com/user-attachments/assets/d12e20dd-3e57-4224-8743-3763c611805d" />
<img width="332" height="622" alt="5" src="https://github.com/user-attachments/assets/85e91326-c559-4d14-ad6b-1aa8927e8933" />
