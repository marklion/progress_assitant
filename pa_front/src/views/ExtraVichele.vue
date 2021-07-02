<template>
<div class="extra_vichele_show" v-if="is_login">
</div>
</template>

<script>
export default {
    name: 'ExtraVichele',
    data: function () {
        return {
            is_login: false,
            user_id: '',
        };
    },
    beforeMount: function () {},
}
</script>

<style>

</style>
