import mongoose from 'mongoose';

const cattleSchema = new mongoose.Schema({
    tagId:{
        type: Number,
        required: true
    },

    name:{
        type: String,
        required: true
    }
});

const Cattle = mongoose.model('cattle',cattleSchema)
export default Cattle;