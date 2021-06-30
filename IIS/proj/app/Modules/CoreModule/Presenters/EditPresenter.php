<?php

namespace App\Modules\CoreModule\Presenters;

use Nette;
use Nette\Application\UI\Form;
use Contributte\FormsBootstrap\BootstrapForm;
use Nette\Utils\Image;


class EditPresenter extends Nette\Application\UI\Presenter
{
    /**
     * @inject
     * @var Nette\Database\Context
     */
    public $database;
    private $user;
    private $hotel;
    private $type;
    
    public function renderEditHotel() : void
    {
        $this->template->rooms =
            $this->database->table('room')->where('hotel_id', $this->hotel);
        $this->template->room_details =
            $this->database->table('room_details')->where('hotel_id', $this->hotel)->fetchAll();
    }
    
    public function actionRole($usr)
    {
        $this->user = $usr;
    }
    
    public function actionAddRoom($hotel)
    {
        $this->hotel = $hotel;
    }
    
    public function renderAddRoom($hotel)
    {
        $this->hotel = $hotel;
    }
    
    public function actionWorkstation($usr)
    {
        $this->user = $usr;
    }
    
    public function actionEditHotel($hotel)
    {
        $this->hotel = $hotel;
    }
    
    public function actionEditRoom($hotel, $type)
    {
        $this->hotel = $hotel;
        $this->type = $type;
        $this->template->room_type = $type;
    }
    
    public function handleRemoveRoomType($hotel_id, $type)
    {
        $this->database->table('room_details')
            ->where('hotel_id', $hotel_id)
            ->where('type', $type)
            ->delete();
    }
    
    public function handleRemoveRoom($hotel_id, $floor_room)
    {
        $this->database->table('room')
            ->where('hotel_id', $hotel_id)
            ->where('floor_room', $floor_room)
            ->delete();
    }
    
    protected function createComponentChangeRoleForm() : Form
    {
        $form = new BootstrapForm;
        
        $form->addInteger('role')
            ->setHtmlAttribute('class','form-control')
            ->setHtmlAttribute('placeholder','User role')
            ->addRule($form::RANGE,null, [1,4])
            ->setRequired('Please enter number from 1-4');

        $form->addSubmit('send', 'Change role');

        $form->onSuccess[] = [$this, 'changeRoleFormSucceeded'];
        return $form;
    }
    
    public function changeRoleFormSucceeded(Form $form, \stdClass $values)
    {
        // if role is receptionist, assign a reception
        if($values->role == 3){
            $this->database->table('receptions')->insert([
            'hotel_id' => 1,
            'guest_mail' => $this->user
        ]);
        }
        
        $this->database->query('UPDATE user SET', [
            'role' => $values->role,
            ],
            'WHERE guest_mail = ?', $this->user);
        
        $this->redirect('Dashboard:');
    }
    
    protected function createComponentAddHotelForm() : Form
    {
        $form = new BootstrapForm;
        
        $form->addText('name')
            ->setHtmlAttribute('class','form-control')
            ->setHtmlAttribute('placeholder','Hotel name')
            ->setRequired('Please enter hotel name.')
            ->setRequired();
        
        $form->addText('city')
            ->setHtmlAttribute('class','form-control')
            ->setHtmlAttribute('placeholder','City')
            ->setRequired();
        
        $form->addText('street')
            ->setHtmlAttribute('class','form-control')
            ->setHtmlAttribute('placeholder','Street')
            ->setRequired();
    
        $form->addText('description')
            ->setHtmlAttribute('class','form-control')
            ->setHtmlAttribute('placeholder','Description')
            ->setRequired();

        $form->addInteger('stars')
            ->setHtmlAttribute('class','form-control')
            ->setHtmlAttribute('placeholder','Stars')
            ->addRule($form::RANGE, null, [0,5])
            ->setRequired();
        
        $form->addText('rating')
            ->setHtmlAttribute('class','form-control')
            ->setHtmlAttribute('placeholder','Rating')
            ->addRule($form::RANGE, null, [0.0,5.0])
            ->addRule($form::FLOAT, 'Value must be a number.')
            ->setRequired();
        
        $form->addUpload('image', 'Hotel image')
            ->setHtmlAttribute('class','form-control')
            ->addRule($form::IMAGE, 'Hotel image has to be JPEG, PNG, GIF or WebP format.')
            ->setRequired('Please put in hotel image.');
        
        $form->addText('gallery')
            ->setHtmlAttribute('class','form-control')
            ->setHtmlAttribute('placeholder','Gallery folder name')
            ->setRequired('Please put in the name of this hotel\'s gallery.');
        
        
        $form->addMultiUpload('gimages', 'Gallery images')
            ->addRule($form::MAX_LENGTH, 'You can upload max %d files', 15)
            ->addRule($form::IMAGE, 'Hotel image has to be JPEG, PNG, GIF or WebP format.')
            ->setRequired('Please put in hotel image.');
    
    
        $form->addSubmit('send', 'Add hotel');
        
        $form->onSuccess[] = [$this, 'addHotelFormSucceeded'];
        return $form;
    }
    
    public function addHotelFormSucceeded(Form $form, \stdClass $values)
    {
        // $path = "../www/img";
        $filename = $values->image->getSanitizedName();
        // $values->image->move($path.'/hotels/'.$filename);

        // rmdir('../www/img/galleries/'.$values->gallery);
        // mkdir('../www/img/galleries/'.$values->gallery);

        // foreach($values->gimages as $img)
        // {
        //     $values->image->move($path.'galleries/'.$img->getSanitizedName());
        // }

        
        

        $row = $this->database->table('hotel')->insert([
            'name' => $values->name,
            'city' => $values->city,
            'street' => $values->street,
            'description' => $values->description,
            'stars' => $values->stars,
            'rating' => $values->rating,
            'image' => $filename,
            'gallery' => $values->gallery
        ]);
        
        $hotel_id = $row->id;
        
        $this->redirect('Edit:addroom', $hotel_id);
    }
    
    protected function createComponentEditHotelForm() : Form
    {
        $form = new BootstrapForm;
        
        $hotel_row = $this->database->table('hotel')
		    ->where('id', $this->hotel)
		    ->fetch();

        $form->addText('name', 'Hotel name:')
            ->setDefaultValue($hotel_row->name)
            ->setRequired('Please enter hotel name.');

        $form->addText('city', 'City:')
            ->setDefaultValue($hotel_row->city)
            ->setRequired('Please enter hotel city.');
        
        $form->addText('street', 'Hotel street:')
            ->setDefaultValue($hotel_row->street)
            ->setRequired('Please enter hotel street.');
        
        $form->addText('description', 'Hotel description:')
            ->setDefaultValue($hotel_row->description)
            ->setRequired('Please enter hotel description.');
        
        $form->addInteger('stars', 'Hotel stars:')
            ->setDefaultValue($hotel_row->stars)
            ->setRequired('Please enter hotel stars.')
            ->addRule($form::RANGE, null, [0,5]);

        $form->addText('rating', 'Hotel rating:')
            ->setDefaultValue($hotel_row->rating)
            ->setRequired('Please enter hotel rating.')
            ->addRule($form::RANGE, null, [0.0,5.0])
            ->addRule($form::FLOAT, 'Value must be a number.');
        
        $form->addUpload('image', 'Hotel image:')
            ->setRequired('Please enter hotel image.')
            ->addRule($form::IMAGE, 'Hotel image has to be JPEG, PNG, GIF or WebP format.');
        
        $form->addText('gallery', 'Hotel gallery:')
            ->setDefaultValue($hotel_row->gallery);

        $form->addMultiUpload('gimages', 'Gallery images')
            ->addRule($form::MAX_LENGTH, 'You can upload max %d files', 15)
            ->addRule($form::IMAGE, 'Hotel image has to be JPEG, PNG, GIF or WebP format.')
            ->setRequired('Please put in hotel image.');

        $form->addSubmit('send', 'Confirm changes');

        $form->onSuccess[] = [$this, 'hotelEditFormSucceeded'];
        
        return $form;
    }
    
    public function hotelEditFormSucceeded(Form $form, \stdClass $values)
    {
        // $path = "../www/img";
        $filename = $values->image->getSanitizedName();
        // $values->image->move($path.'/hotels/'.$filename);

        // rmdir('../www/img/galleries/'.$values->gallery);
        // mkdir('../www/img/galleries/'.$values->gallery);

        // foreach($values->gimages as $img)
        // {
        //     $values->image->move($path.'galleries/'.$img->getSanitizedName());
        // }

        $this->database->query('UPDATE hotel SET', [
            'name' => $values->name,
            'city' => $values->city,
            'street' => $values->street,
            'description' => $values->description,
            'stars' => $values->stars,
            'rating' => $values->rating,
            'image' => $filename,
            'gallery' => $values->gallery
            ], 'WHERE id = ?', $this->hotel);
    
        $this->redirect('Dashboard:');
    }
    
    protected function createComponentAddRoomForm() : Form
    {
        $form = new BootstrapForm;

        $form->addText('type')
            ->setHtmlAttribute('class','form-control')
            ->setHtmlAttribute('placeholder','Room type')
            ->setRequired('Please enter room type.');

        $form->addInteger('floor_room')
            ->setHtmlAttribute('class','form-control')
            ->setHtmlAttribute('placeholder','Room number')
            ->setRequired('Please enter room number.');

        $form->addSubmit('send', 'Add room');

        $form->onSuccess[] = [$this, 'addRoomFormSucceeded'];
        
        return $form;
        
    }
    
    public function addRoomFormSucceeded(Form $form, \stdClass $values)
    {
        // insert room into database
        $this->database->table('room')->insert([
            'type' => $values->type,
            'floor_room' => $values->floor_room,
            'hotel_id' => $this->hotel,
        ]);
        
        // get room types of this hotel to an array
        $room_details_arr =
            $this->database->table('room_details')
                ->where('hotel_id', $this->hotel)
                ->where('type', $values->type)
                ->fetch();

        //if room type is not already used, add it to room details
        if (!$room_details_arr) {
            $this->database->table('room_details')->insert
            ([
                'type' => $values->type,
                'hotel_id' => $this->hotel
            ]);
        }
        
        $this->redirect('Edit:edithotel', $this->hotel);
    }
    
    protected function createComponentEditRoomForm() : Form
    {
        $form = new BootstrapForm;
        
        $room_row = $this->database->table('room_details')
		        ->where('hotel_id', $this->hotel)
                        ->where('type', $this->type)
		        ->fetch();

        $form->addInteger('beds')
            ->setHtmlAttribute('class','form-control')
            ->setHtmlAttribute('placeholder','Number of beds')
            ->setRequired()
            ->setDefaultValue($room_row->beds);
            

        $form->addText('facility')
            ->setHtmlAttribute('class','form-control')
            ->setHtmlAttribute('placeholder','Facilities')
            ->setDefaultValue($room_row->facility)
            ->setRequired();

        $form->addInteger('price')
            ->setHtmlAttribute('class','form-control')
            ->setHtmlAttribute('placeholder','Price')
            ->setDefaultValue($room_row->price)
            ->setRequired();
        
        $form->addUpload('image', 'Room image')
            ->addRule($form::IMAGE, 'Rooom has to be JPEG, PNG, GIF or WebP format.')
            ->setRequired('Please put in room picture.');
        
        $form->addSubmit('send', 'Confirm changes');
        
        $form->onSuccess[] = [$this, 'editRoomFormSucceeded'];
        
        return $form;
    }
    
    public function editRoomFormSucceeded(Form $form, \stdClass $values)
    {

        // $path = "../www/img";
        $filename = $values->image->getSanitizedName();
        // $values->image->move($path.'/hotels/'.$filename);

        $this->database->query('UPDATE room_details SET', [
            'beds' => $values->beds,
            'facility' => $values->facility,
            'price' => $values->price,
            'image' => $filename,
            ],
            'WHERE hotel_id = ?', $this->hotel,
                'AND type = ?', $this->type);
    
        $this->redirect('Edit:edithotel', $this->hotel);
    }
    
    protected function createComponentWorkstationForm() : Form
    {
        $form = new BootstrapForm;

        $form->addInteger('id')
            ->setHtmlAttribute('class','form-control')
            ->setHtmlAttribute('placeholder','New ID')
            ->addRule($form::RANGE,'Invalid role ID', [1,4])
            ->setRequired('Please enter this receptionist\'s new hotel ID.');
        
        $form->addSubmit('send', 'Confirm changes');
        
        $form->onSuccess[] = [$this, 'workstationFormSucceeded'];
        
        return $form;
    }
    
    public function workstationFormSucceeded(Form $form, \stdClass $values)
    {
        $this->database->query('UPDATE receptions SET', [
            'hotel_id' => $values->id,
            ],
            'WHERE guest_mail = ?', $this->user);
    
        $this->redirect('Dashboard:');
    }
}
