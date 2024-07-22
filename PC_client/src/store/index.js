import Vue from 'vue';
import Vuex from 'vuex';

Vue.use(Vuex);

export default new Vuex.Store({
  state: {
    CO_value: 10,
    CO2_value: 55,
    CH4_value: 12,
    H2_value: 18,
    switch_CO: true,
    switch_CO2: true,
    switch_CH4: true,
    switch_H2: true,
  },
  mutations: {
    setCOValue(state, value) {
      state.CO_value = value;
    },
    setCO2Value(state, value) {
      state.CO2_value = value;
    },
    setCH4Value(state, value) {
      state.CH4_value = value;
    },
    setH2Value(state, value) {
      state.H2_value = value;
    },
    setSwitchCO(state, value) {
      state.switch_CO = value;
    },
    setSwitchCO2(state, value) {
      state.switch_CO2 = value;
    },
    setSwitchCH4(state, value) {
      state.switch_CH4 = value;
    },
    setSwitchH2(state, value) {
      state.switch_H2 = value;
    },
  },
});
