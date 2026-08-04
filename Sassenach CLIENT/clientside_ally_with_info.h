#ifndef CLIENTSIDE_ALLY_WITH_INFO_H_INCLUDED
#define CLIENTSIDE_ALLY_WITH_INFO_H_INCLUDED

    int client_IP_number = 0;
    char* client_IP[100];

    string ip_address_input;
    string ip_label;
    string port_input;
    string client_identity;

    bool ingame = false;
    int tot_players = 0;
    bool stuck_in_purgatory = true;

    int jump_budget = 0;
    int maximum_jump_budget = 7;

#endif // CLIENTSIDE_ALLY_WITH_INFO_H_INCLUDED
