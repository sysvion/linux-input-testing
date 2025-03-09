// Datastructs
#include <complex>
#include <set>

#include <csignal> // for ctrl-c

#include <iostream> // cout <<
#include <cstdio> // printf & scan etc

// file operations
#include <unistd.h>
#include <fcntl.h>

#include <linux/input.h>
#include <linux/input-event-codes.h>

#include <sys/time.h>

void print_event_details(struct input_event *data ) {
    std::cout << "\tcode: " << std::hex << data->code << std::endl
              << "\tval:"   << std::dec << data->value << std::endl;
}


std::set<int> keys = std::set<int>();
std::set<int> unhandeld_event;
int fp = 0;

void sigint_handeler(int signal) {
    try {
        close(fp);
        std::cout << "keys found\r\n";
        for (int i : keys ) {
            std::cout << i << std::endl; 
        }
    } catch (int er) {}
    exit(0);
}

int main(int argc, char **argv) {

    int fp = open("/dev/input/event8", O_RDONLY);
    if (!fp) {
        std::cerr << "could not open input";
        return 1;
    }

    int is_ok = EXIT_SUCCESS;
    //void * buff = malloc(sizeof(struct input_event));


    signal(SIGINT, sigint_handeler);

    while (is_ok != EXIT_FAILURE) {

        char buff[sizeof(struct input_event)];
        int err = read(fp, buff, sizeof(struct input_event));


        if (err == -1) {
            std::puts("I/O error when reading");
            break;
        }

        struct input_event *incoming = (struct input_event *) buff;


        if (incoming->type == EV_REL ) {
            continue;
        }

        std::cout << "time: " << incoming->time.tv_sec << std::endl;
        switch (incoming->type) {
            case EV_SYN:
                // only report... continue...
                break;
            case  EV_KEY:
                std::cout << "EV_KEY:" << incoming->value << std::endl;
                print_event_details(incoming);
                keys.insert(incoming->code);
                // 272
                // 325
                // 330
                // 333
                // 334
                break;
            case  EV_REL:
                // i currently only use relative.
                
                break;
            case  EV_ABS:
                std::cout << "EV_ABS:\r\n";
                
                switch (incoming->code) {
                    case ABS_X:
                        std::cout << "  move x by " << incoming->value << std::endl; 
                        break;

                    case ABS_Y:
                        std::cout << "  move Y by " << incoming->value << std::endl; 
                        break;

                    case ABS_MT_POSITION_X:
                        std::cout << " center x is " << incoming->value << std::endl; 
                        break;

                    case ABS_MT_POSITION_Y:
                        std::cout << " center y is " << incoming->value << std::endl; 
                        break;

                    default:
                        std::cout << "  code: " << std::hex << incoming->code << std::endl << "  val: " << std::dec << incoming->value << std::endl;
                        break;
                }
                break;
            case  EV_MSC:
                std::cout << "EV_MSC" << std::endl;
                print_event_details(incoming);
                break;
            case  EV_SW:
                std::cout << "EV_SW" << std::endl;
                print_event_details(incoming);
                break;
            case  EV_LED:
                std::cout << "EV_LED" << std::endl;
                print_event_details(incoming);
                break;
            case  EV_SND:
                std::cout << "EV_SND" << std::endl;
                print_event_details(incoming);
                break;
            case  EV_REP:
                std::cout << "EV_REP" << std::endl;
                print_event_details(incoming);
                break;
            case  EV_FF:
                std::cout << "EV_FF" << std::endl;
                print_event_details(incoming);
                break;
            case  EV_PWR:
                std::cout << "EV_PWR" << std::endl;
                print_event_details(incoming);
                break;
            case  EV_FF_STATUS:
                std::cout << "EV_FF_STATUS" << std::endl;
                print_event_details(incoming);
                break;
            case  EV_MAX:
                std::cout << "EV_MAX" << std::endl;
                print_event_details(incoming);
                break;
            case  EV_CNT:
                std::cout << "EV_CNT" << std::endl;
                print_event_details(incoming);
                break;
            default:
                unhandeld_event.insert(incoming->code);
                break;
                

        }

         std::cout << std::endl;
    

    }

    sigint_handeler(0);
    return is_ok;
}
