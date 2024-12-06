#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include "fsm.h"

typedef struct bit_counter_
{
    unsigned int count_1;
    unsigned int count_0;
} bit_counter_t;

void bit_flipper_fn_gen(state_t *from, state_t *to,
                        char *input_buff,
                        unsigned int input_buff_size,
                        fsm_output_buff_t *fsm_output_buff)
{
    char out;
    if (fsm_output_buff->curr_pos == 0)
    {
        fsm_output_buff->curr_pos = sizeof(bit_counter_t);
    }

    bit_counter_t *stats_space = (bit_counter_t *)fsm_output_buff->output_buffer;

    if (*input_buff == '1')
    {
        out = '0';
        stats_space->count_1++;
    }
    else
    {
        out = '1';
        stats_space->count_0++;
    }

    fsm_output_buff->curr_pos += snprintf(fsm_output_buff->output_buffer +
                                              fsm_output_buff->curr_pos,
                                          (MAX_FSM_OUTPUT_BUFFER - fsm_output_buff->curr_pos - 1),
                                          "%s-->%c|-->%s\n",
                                          from->state_name, *input_buff, out,
                                          to->state_name);
}
int main(int argc, char **argv)
{
    fsm_t *fsm = create_new_fsm("Bit Flipper");
    state_t *state_S0 = create_new_state("S0", FSM_TRUE);
    set_fsm_initial_state(fsm, state_S0);

    char bit = '0';
    create_and_insert_new_tt_entry(&state_S0->state_trans_table,
                                   &bit, 1,
                                   bit_flipper_fn_gen,
                                   state_S0);

    bit = '1';
    create_and_insert_new_tt_entry(&state_S0->state_trans_table,
                                   &bit, 1,
                                   bit_flipper_fn_gen,
                                   state_S0);

    fsm_bool_t fsm_result;
    fsm_error_t fsm_error;

    fsm_error = execute_fsm(fsm,
                            "0000000\0",
                            strlen("0000000\0"),
                            0,
                            &fsm_result);

    if (fsm_error == FSM_NO_ERROR)
    {
        printf("FSM result = %s\n", fsm_result == FSM_TRUE ? "FSM_TRUE" : "FSM_FALSE");
        printf("FSM Output string : \n%s\n", fsm->fsm_output_buff.output_buffer + sizeof(bit_counter_t));
        printf("FSM Output Stats : #of 1's = %u, #of 0's = %u\n",
               ((bit_counter_t *)fsm->fsm_output_buff.output_buffer)->count_1,
               ((bit_counter_t *)fsm->fsm_output_buff.output_buffer)->count_0);
    }

    fsm_output_buff_t fsm_output_buff;
    init_fsm_output_buffer(&fsm_output_buff);

    fsm_error = execute_fsm(fsm,
                            "1111111\0",
                            strlen("1111111\0"),
                            0,
                            &fsm_result);

    if (fsm_error == FSM_NO_ERROR)
    {
        printf("FSM result = %s\n", fsm_result == FSM_TRUE ? "FSM_TRUE" : "FSM_FALSE");
        printf("FSM Output string : \n%s\n", fsm->fsm_output_buff.output_buffer + sizeof(bit_counter_t));
        printf("FSM Output Stats : #of 1's = %u, #of 0's = %u\n",
               ((bit_counter_t *)fsm->fsm_output_buff.output_buffer)->count_1,
               ((bit_counter_t *)fsm->fsm_output_buff.output_buffer)->count_0);
    }

    /*Assign the input buffer to FSM execute */
    strncpy(fsm->input_buffer, "0101010101010\0", strlen("0101010101010"));
    set_fsm_input_buffer_size(fsm, strlen("0101010101010\0"));
    fsm_error = execute_fsm(fsm,
                            0,
                            0,
                            &fsm_output_buff,
                            &fsm_result

    );

    if (fsm_error == FSM_NO_ERROR)
    {
        printf("FSM result = %s\n", fsm_result == FSM_TRUE ? "FSM_TRUE" : "FSM_FALSE");
        printf("FSM Output string : \n%s\n", fsm->fsm_output_buff.output_buffer + sizeof(bit_counter_t));
        printf("FSM Output Stats : #of 1's = %u, #of 0's = %u\n",
               ((bit_counter_t *)fsm->fsm_output_buff.output_buffer)->count_1,
               ((bit_counter_t *)fsm->fsm_output_buff.output_buffer)->count_0);
    }

    return 0;

}