<template>
<div>
    <ticket-content :ticket_id="ticket_id"></ticket-content>
</div>
</template>

<script>
import TicketContent from '../components/TicketContent.vue'
export default {
    name: 'Ticket',
    components: {
        'ticket-content': TicketContent,
    },
    data: function () {
        return {
            ticket_id: '',
        };
    },
    beforeMount: function () {
        this.ticket_id = this.$route.params.id;
    },
}
</script>

<style>

</style>
