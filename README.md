# PODEM-and-Deductive-Fault-Simulator

## Usage
    1. Unzip the file.
    2. Add the fault that is required to test to “label”_podem_fault.txt. For example, s298f_2_podem_fault.txt.
    3. Change the Address in the Main.cpp. Make sure they can find the file.
    4. Use the command “g++ -o program Main.cpp Gatesim.cpp Deductive.cpp In_out.cpp Podem.cpp PodemSim.cpp”
    5. Use the command “./program“
    6. Type B for PODEM
    7. Input the label you want to test like s27.
