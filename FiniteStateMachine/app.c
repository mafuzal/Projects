#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include "fsm.h"
#include "std_fsm.h"

void bit_flipper_output_fn_gen(state_t *from, state_t *to,
                               char *input_buff,
                               unsigned int input_buff_size,
                               fsm_output_buff_t *fsm_output_buff)
{
    char out;
    out = (*input_buff == '1') ? '0' : '1';
    fsm_output_buff->curr_pos += snprintf(fsm_output_buff->output_buffer +
                                              fsm_output_buff->curr_pos,
                                          (MAX_FSM_OUTPUT_BUFFER - fsm_output_buff->curr_pos - 1),
                                          "%s-->%c|%c-->%s\n",
                                          from->state_name, *input_buff, out,
                                          to->state_name);
}

int main(int argc, char **argv)
{

    /*Create a FSM */
    fsm_t *fsm = create_new_fsm("Bit Flipper");

    /*Create FSM State*/
    state_t *state_S0 = create_new_state("S0", FSM_TRUE);
    // state_t *state_S0 = create_new_state(fsm, "S0", FSM_TRUE, bit_flipper_key_match_fn);

    /*Set FSM initial state */
    set_fsm_initial_state(fsm, state_S0);

    /* Insert Transitions into State's Transition Table*/
    char bit = '0';
    create_and_insert_new_tt_entry(&state_S0->state_trans_table,
                                   &bit, 1,
                                   bit_flipper_output_fn_gen,
                                   state_S0);

    bit = '1';
    create_and_insert_new_tt_entry(&state_S0->state_trans_table,
                                   &bit, 1,
                                   bit_flipper_output_fn_gen,
                                   state_S0);

    /*
     * FSM creation has been complete, now let us see how our FSM perform
     */

    fsm_bool_t fsm_result;
    fsm_error_t fsm_error;

    fsm_error = execute_fsm(fsm,
                            "0000000\0",
                            strlen("0000000\0"),
                            0,
                            &fsm_result);

    if (fsm_erro == FSM_NO_ERROR)
    {
        printf("FSM result = %s\n", fsm_result == FSM_TRUE ? "FSM_TRUE" : "FSM_FALSE");
        printf("FSM Output string : \n%s\n", fsm_output_buff.output_buffer);
    }

    /*FSM for email Validation */
    fsm_t *email_validator = email_validator_fsm();
    fsm_error = execute_fsm(email_validator,
                            "TBA\0",
                            strlen("TBA\0"),
                            0,
                            &fsm_result);

    if (fsm_error == FSM_NO_ERROR)
    {
        if (fsm_result == FSM_TRUE)
            printf("Valid email\n");
        else
            printf("Invalid Email\n")
    }
    else
    {
        printf("FSM State Machine Failed\n");
    }



    fsm_t *fsm_bin_to_hex = fsm_binary_to_hex();
    fsm_error = execute_fsm(fsm_bin_to_hex,
                            "00011111111111111111111111111111\0",
                            strlen("00011111111111111111111111111111\0"),
                            0, 0);

    if(fsm_error == FSM_NO_ERROR)
    {
        printf("Hex = %s\n",fsm_bin_to_hex->fsm_output_buff.output_buffer);
    }

    


}